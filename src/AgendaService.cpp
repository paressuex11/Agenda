#include "AgendaService.hpp"
#include <algorithm>
#include "Exception.hpp"

using std::find;
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
AgendaService::~AgendaService() { this->quitAgenda(); }

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const string &userName, const string &password) {
  auto filter = [&userName, &password](const User &u) -> bool {
    return u.getName() == userName && u.getPassword() == password;
  };

  if (this->m_storage->queryUser(filter).empty())
    throw user_not_found("User: " + userName);

  return true;
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
  auto filter = [&userName](const User &u) -> bool {
    return u.getName() == userName;
  };

  if (!this->m_storage->queryUser(filter).empty())
    throw user_repeat("Username: " + userName + " is taken by others");

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
  auto filterUserExist = [&userName, &password](const User &u) -> bool {
    return u.getName() == userName && u.getPassword() == password;
  };

  if (this->m_storage->deleteUser(filterUserExist) == 0)
    throw user_not_found("User: " + userName);

  this->m_storage->updateMeeting(
      [&userName](const Meeting &m) -> bool {
        return m.isParticipator(userName);
      },
      [&userName](Meeting &m) { m.removeParticipator(userName); });
  this->m_storage->deleteMeeting([&userName](const Meeting &m) -> bool {
    return m.getSponsor() == userName || m.getParticipator().empty();
  });

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
  if (!Date::isValid(sDate)) throw invalid_date("Start date: " + startDate);

  if (!Date::isValid(eDate)) throw invalid_date("End date: " + endDate);

  if (sDate >= eDate)
    throw invalid_date("Start date must be earlier than end date");

  auto filterSponsorExist = [&userName](const User &u) -> bool {
    return u.getName() == userName;
  };

  // check if sponsor exists
  if (this->m_storage->queryUser(filterSponsorExist).empty())
    throw user_not_found("Sponsor: " + userName);

  for (auto it = participator.begin(); it != participator.end(); it++) {
    // check if sponsor is one of participators
    if (userName == *it)
      throw user_repeat("Sponsor: " + userName + " is found in participators");

    string part = *it;

    auto filterParticipatorExist = [&part](const User &u) -> bool {
      return u.getName() == part;
    };

    // check if participator exists
    if (this->m_storage->queryUser(filterParticipatorExist).empty())
      throw user_not_found("Participator: " + *it);

    // check if participator repeats
    for (auto itInner = participator.begin(); itInner != it; itInner++) {
      if (*itInner == *it)
        throw user_repeat("Participator: " + *it + " appears more than once");
    }
  }

  auto filterOverlapAndBusy = [&userName, &title, &sDate, &eDate,
                               &participator](const Meeting &m) -> bool {
    // check if title is repeated
    if (m.getTitle() == title)
      throw title_repeat("Meeting sponsored by " + m.getSponsor() +
                         " has a same title");

    // check if sponsor is busy
    if (m.getSponsor() == userName || m.isParticipator(userName)) {
      bool isBusy = false;

      if (m.getStartDate() <= sDate && m.getEndDate() > sDate) isBusy = true;

      if (m.getStartDate() < eDate && m.getEndDate() >= eDate) isBusy = true;

      if (m.getStartDate() >= sDate && m.getEndDate() <= eDate) isBusy = true;

      if (isBusy)
        throw time_conflict("Sponsor: " + userName + " is busy from " +
                            Date::dateToString(m.getStartDate()) + " to " +
                            Date::dateToString(m.getEndDate()));
    }

    // check if any participator is busy
    for (const string &part : participator) {
      if (m.getSponsor() == part || m.isParticipator(part)) {
        bool isBusy = false;

        if (m.getStartDate() <= sDate && m.getEndDate() > sDate) isBusy = true;

        if (m.getStartDate() < eDate && m.getEndDate() >= eDate) isBusy = true;

        if (m.getStartDate() >= sDate && m.getEndDate() <= eDate) isBusy = true;

        if (isBusy)
          throw time_conflict("Participator: " + part + " is busy from " +
                              Date::dateToString(m.getStartDate()) + " to " +
                              Date::dateToString(m.getEndDate()));
      }
    }

    return false;
  };

  this->m_storage->queryMeeting(filterOverlapAndBusy);
  this->m_storage->createMeeting(
      Meeting(userName, (participator), startDate, endDate, title));

  return true;
}

/**
 * add a participator to a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
  auto filterMeetingExist = [&userName, &title](const Meeting &m) -> bool {
    if (m.getTitle() == title && m.getSponsor() == userName) return true;
    return false;
  };
  auto meetings = this->m_storage->queryMeeting(filterMeetingExist);

  if (meetings.empty())
    throw meeting_not_found("Title: " + title + ". Sponsor: " + userName);

  auto filterParticipatorExist = [&participator](const User &u) -> bool {
    return u.getName() == participator;
  };

  if (this->m_storage->queryUser(filterParticipatorExist).empty())
    throw user_not_found("Participator: " + participator);

  auto meeting = meetings.front();
  auto filterOverlap = [&participator, &meeting](const Meeting &m) -> bool {
    if (m.getSponsor() == participator || m.isParticipator(participator)) {
      bool isBusy = false;

      if (m.getStartDate() <= meeting.getStartDate() &&
          m.getEndDate() > meeting.getStartDate())
        isBusy = true;
      if (m.getStartDate() < meeting.getEndDate() &&
          m.getEndDate() >= meeting.getEndDate())
        isBusy = true;
      if (m.getStartDate() >= meeting.getStartDate() &&
          m.getEndDate() <= meeting.getEndDate())
        isBusy = true;

      if (isBusy)
        throw time_conflict("Participator: " + participator + " is busy from " +
                            Date::dateToString(m.getStartDate()) + " to " +
                            Date::dateToString(m.getEndDate()));
    }

    return false;
  };

  this->m_storage->queryMeeting(filterOverlap);
  this->m_storage->updateMeeting(
      [&meeting](const Meeting &m) {
        return m.getTitle() == meeting.getTitle();
      },
      [&participator](Meeting &m) { m.addParticipator(participator); });

  return true;
}

/**
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
  auto filterMeetingExist = [&userName, &title](const Meeting &m) -> bool {
    if (m.getTitle() == title && m.getSponsor() == userName) return true;
    return false;
  };
  auto meetings = this->m_storage->queryMeeting(filterMeetingExist);

  if (meetings.empty())
    throw meeting_not_found("Title: " + title + ". Sponsor: " + userName);

  auto filterParticipatorExist = [&participator](const User &u) -> bool {
    return u.getName() == participator;
  };

  auto meeting = meetings.front();
  auto participators = meeting.getParticipator();

  if (find(participators.begin(), participators.end(), participator) ==
      participators.end())
    throw user_not_found("Participator: " + participator);

  this->m_storage->updateMeeting(
      [&meeting](const Meeting &m) {
        if (m.getTitle() == meeting.getTitle()) return true;
        return false;
      },
      [&participator](Meeting &m) { m.removeParticipator(participator); });
  this->m_storage->deleteMeeting([&meeting](const Meeting &m) {
    return m.getParticipator().empty() && m.getTitle() == meeting.getTitle();
  });

  return true;
}

/**
 * quit from a meeting
 * @param userName the current userName. need to be the participator (a
 * sponsor can not quit his/her meeting)
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName,
                                const std::string &title) {
  auto filterMeetingExist = [&userName, &title](const Meeting &m) -> bool {
    return m.getTitle() == title && m.isParticipator(userName);
  };
  auto meetings = this->m_storage->queryMeeting(filterMeetingExist);

  // check if meeting exists
  if (meetings.empty())
    throw meeting_not_found("Title: " + title + ". Participator: " + userName);

  Meeting meeting = meetings.front();

  this->m_storage->updateMeeting(
      [&meeting](const Meeting &m) -> bool {
        return m.getTitle() == meeting.getTitle();
      },
      [&userName](Meeting &m) { m.removeParticipator(userName); });
  this->m_storage->deleteMeeting([&meeting](const Meeting &m) -> bool {
    return m.getParticipator().empty() && m.getTitle() == meeting.getTitle();
  });

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
  auto filter = [&userName, &title](const Meeting &m) -> bool {
    return (m.getSponsor() == userName || m.isParticipator(userName)) &&
           m.getTitle() == title;
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

  if (!Date::isValid(sDate)) throw invalid_date("Start date: " + startDate);

  if (!Date::isValid(eDate)) throw invalid_date("End date: " + endDate);

  if (sDate > eDate)
    throw invalid_date("Start date must be earlier than end date");

  auto filter = [&userName, &sDate, &eDate](const Meeting &m) -> bool {
    return !(eDate < m.getStartDate() || sDate > m.getEndDate()) &&
           (m.getSponsor() == userName || m.isParticipator(userName));
  };

  return this->m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
list<Meeting> AgendaService::listAllMeetings(const string &userName) const {
  auto filter = [&userName](const Meeting &m) -> bool {
    return m.getSponsor() == userName || m.isParticipator(userName);
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
  auto filter = [&userName](const Meeting &m) -> bool {
    return m.getSponsor() == userName;
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
  auto filter = [&userName](const Meeting &m) -> bool {
    return m.isParticipator(userName);
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
  auto filter = [&userName, &title](const Meeting &m) -> bool {
    return m.getSponsor() == userName && m.getTitle() == title;
  };

  if (this->m_storage->deleteMeeting(filter) == 0)
    throw empty_deletion("No meeting found. Title: " + title +
                         ". Sponsor: " + userName);

  return true;
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const string &userName) {
  auto filter = [&userName](const Meeting &m) -> bool {
    return m.getSponsor() == userName;
  };

  if (this->m_storage->deleteMeeting(filter) == 0)
    throw empty_deletion("No meeting found. Sponsor: " + userName);

  return true;
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