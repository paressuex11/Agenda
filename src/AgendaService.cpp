#include "AgendaService.hpp"

using std::list;
using std::string;
using std::vector;

/**
 * constructor
 */
AgendaService::AgendaService() { this->startAgenda(); };

/**
 * destructor
 */
AgendaService::~AgendaService() {
  if (this->m_storage != nullptr) this->quitAgenda();
}

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const string &userName, const string &password) {
  auto filter = [&](const User &user) -> bool {
    return user.getName() == userName && user.getPassword() == password;
  };

  return !this->m_storage->queryUser(filter).empty();
}

/**
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const string &userName, const string &password,
                                 const string &email, const string &phone) {
  auto filter = [&](const User &user) -> bool {
    return user.getName() == userName;
  };

  if (!this->m_storage->queryUser(filter).empty()) return false;

  this->m_storage->createUser(User(userName, password, email, phone));
  return true;
}

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const string &userName, const string &password) {
  auto filter = [&](const User &user) -> bool {
    return user.getName() == userName && user.getPassword() == password;
  };

  if (this->m_storage->deleteUser(filter) == 0) return false;

  auto remover = [&](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName || meeting.isParticipator(userName);
  };

  this->m_storage->deleteMeeting(remover);
  return true;
}

/**
 * list all users from storage
 * @return a user list result
 */
list<User> AgendaService::listAllUsers(void) const {
  auto filter = [](const User &) -> bool { return true; };

  return this->m_storage->queryUser(filter);
}

/**
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startData the meeting's start date
 * @param endData the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const string &userName, const string &title,
                                  const string &startDate,
                                  const string &endDate,
                                  const vector<string> &participator) {
  Date sDate = Date::stringToDate(startDate);
  Date eDate = Date::stringToDate(endDate);

  // check if dates valid
  if (!Date::isValid(sDate) || !Date::isValid(eDate) || sDate >= eDate) {
    return false;
  }

  auto filterSponsorExist = [&](const User &user) -> bool {
    return user.getName() == userName;
  };

  // check if sponsor exists
  if (this->m_storage->queryUser(filterSponsorExist).empty()) return false;

  for (auto it = participator.begin(); it != participator.end(); it++) {
    // check if sponsor is one of participators
    if (userName == *it) return false;

    auto filterParticipatorExist = [&](const User &user) -> bool {
      return user.getName() == *it;
    };

    // check if participator exists
    if (this->m_storage->queryUser(filterParticipatorExist).empty())
      return false;

    // check if participator repeats
    for (auto itInner = participator.begin(); itInner != it; itInner++) {
      if (*itInner == *it) return false;
    }
  }

  auto filterOverlapAndBusy = [&](const Meeting &meeting) -> bool {
    // check if title is repeated
    if (meeting.getTitle() == title) return true;

    // check if sponsor is busy
    if (userName == meeting.getSponsor() || meeting.isParticipator(userName)) {
      if (meeting.getStartDate() <= sDate && meeting.getEndDate() > sDate)
        return true;

      if (meeting.getStartDate() < eDate && meeting.getEndDate() >= eDate)
        return true;

      if (meeting.getStartDate() >= sDate && meeting.getEndDate() <= eDate)
        return true;
    }

    // check if any participator is busy
    for (const string &part : participator) {
      if (part == meeting.getSponsor() || meeting.isParticipator(part)) {
        if (meeting.getStartDate() <= sDate && meeting.getEndDate() > sDate)
          return true;

        if (meeting.getStartDate() < eDate && meeting.getEndDate() >= eDate)
          return true;

        if (meeting.getStartDate() >= sDate && meeting.getEndDate() <= eDate)
          return true;
      }
    }

    return false;
  };

  if (!this->m_storage->queryMeeting(filterOverlapAndBusy).empty())
    return false;

  this->m_storage->createMeeting(
      Meeting(userName, (participator), startDate, endDate, title));
  return true;
}

/**
 * search a meeting by username and title
 * @param userName as a sponsor OR a participator
 * @param title the meeting's title
 * @return a meeting list result
 */
list<Meeting> AgendaService::meetingQuery(const string &userName,
                                          const string &title) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName ||
            meeting.isParticipator(userName)) &&
           meeting.getTitle() == title;
  };

  return this->m_storage->queryMeeting(filter);
}

/**
 * search a meeting by username, time interval
 * @param userName as a sponsor OR a participator
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
list<Meeting> AgendaService::meetingQuery(const string &userName,
                                          const string &startDate,
                                          const string &endDate) const {
  Date sDate = Date::stringToDate(startDate);
  Date eDate = Date::stringToDate(endDate);

  list<Meeting> listMeeting;

  if (!Date::isValid(sDate) || !Date::isValid(eDate) || sDate > eDate) {
    return listMeeting;
  }

  auto filter = [&](const Meeting &meeting) -> bool {
    return !(eDate < meeting.getStartDate() || sDate > meeting.getEndDate()) &&
           (meeting.getSponsor() == userName ||
            meeting.isParticipator(userName));
  };

  return this->m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
list<Meeting> AgendaService::listAllMeetings(const string &userName) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName || meeting.isParticipator(userName);
  };

  return this->m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
list<Meeting> AgendaService::listAllSponsorMeetings(
    const string &userName) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName;
  };

  return this->m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
list<Meeting> AgendaService::listAllParticipateMeetings(
    const string &userName) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return meeting.isParticipator(userName);
  };

  return this->m_storage->queryMeeting(filter);
}

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const string &userName, const string &title) {
  auto filter = [&](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName && meeting.getTitle() == title;
  };

  return this->m_storage->deleteMeeting(filter) != 0;
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const string &userName) {
  auto filter = [&](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName;
  };

  return this->m_storage->deleteMeeting(filter) != 0;
}

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
  this->m_storage = Storage::getInstance();
}

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
  this->m_storage->sync();
  this->m_storage = nullptr;
}