#include "Storage.hpp"
#include <fstream>  // ifstream ostream
#include <regex>    // reges expression
#include "Exception.hpp"
#include "Path.hpp"

using std::function;
using std::list;
using std::string;

std::shared_ptr<Storage> Storage::m_instance = nullptr;

/**
 *  default constructor
 */
Storage::Storage() : m_dirty(false) { this->readFromFile(); }

/**
 * read file content into memory
 * @return if success, true will be returned
 */
bool Storage::readFromFile(void) {
  std::ifstream userStream(Path::userPath);

  if (userStream.fail()) return false;

  std::ifstream meetingStream(Path::meetingPath);

  if (meetingStream.fail()) return false;

  std::smatch result;

  // <username>,<password>,<email>,<phone>
  std::regex userCSVPattern("^\"(.+)\",\"(.+)\",\"(.+)\",\"(.+)\"$");

  for (string line; std::getline(userStream, line);) {
    if (!std::regex_match(line, result, userCSVPattern))
      throw wrong_format("Wrong User CSV Format");

    this->m_userList.push_back(
        User(result[1], result[2], result[3], result[4]));
  }

  userStream.close();

  // <sponsor>,<participators>,<start date>,<end date>,<title>
  std::regex meetingCSVPattern(
      "^\"(.+)\",\"(.+)\",\"(.+)\",\"(.+)\",\"(.+)\"$");

  for (string line; std::getline(meetingStream, line);) {
    if (!std::regex_match(line, result, meetingCSVPattern))
      throw wrong_format("Wrong Meeting CSV Format");

    Meeting t_meeting;

    t_meeting.setSponsor(result[1]);

    string participators = result[2];
    std::vector<string> t_participators;
    size_t pos = 0;
    string delimiter = "&";

    // Split a string using delimiter
    while ((pos = participators.find(delimiter)) != string::npos) {
      t_participators.push_back(participators.substr(0, pos));
      participators.erase(0, pos + delimiter.length());
    }

    t_participators.push_back(participators);
    t_meeting.setParticipator(t_participators);
    t_meeting.setStartDate(Date::stringToDate(string(result[3])));
    t_meeting.setEndDate(Date::stringToDate(string(result[4])));
    t_meeting.setTitle(result[5]);

    this->m_meetingList.push_back(t_meeting);
  }

  meetingStream.close();

  return true;
}

/**
 * convert the participators list to string
 * @param the source participators list
 * @return the result string
 */
string vectorToString(const std::vector<string> &participators) {
  string result = "";
  bool isFirstItem = true;

  for (auto it = participators.begin(); it != participators.end(); it++) {
    if (isFirstItem) {
      isFirstItem = false;
    } else {
      result += "&";
    }

    result += *it;
  }

  return result;
}

/**
 * write file content from memory
 * @return if success, true will be returned
 */
bool Storage::writeToFile(void) {
  std::ofstream userStream(Path::userPath);

  if (userStream.fail()) return false;

  std::ofstream meetingStream(Path::meetingPath);

  if (meetingStream.fail()) return false;

  for (const User &user : this->m_userList) {
    userStream << "\"" << user.getName() << "\",";
    userStream << "\"" << user.getPassword() << "\",";
    userStream << "\"" << user.getEmail() << "\",";
    userStream << "\"" << user.getPhone() << "\"\n";
  }

  userStream.close();

  for (const Meeting &meeting : this->m_meetingList) {
    meetingStream << "\"" << meeting.getSponsor() << "\",";
    meetingStream << "\"" << vectorToString(meeting.getParticipator()) << "\",";
    meetingStream << "\"" << Date::dateToString(meeting.getStartDate())
                  << "\",";
    meetingStream << "\"" << Date::dateToString(meeting.getEndDate()) << "\",";
    meetingStream << "\"" << meeting.getTitle() << "\"\n";
  }

  meetingStream.close();

  this->m_dirty = false;

  return true;
}

/**
 * get Instance of storage
 * @return the pointer of the instance
 */
std::shared_ptr<Storage> Storage::getInstance(void) {
  if (Storage::m_instance == nullptr)
    Storage::m_instance = std::shared_ptr<Storage>(new Storage);

  return m_instance;
}

/**
 * destructor
 */
Storage::~Storage() {
  if (this->m_dirty) this->writeToFile();
}

/**
 * create a user
 * @param a user object
 */
void Storage::createUser(const User &t_user) {
  this->m_userList.push_back(t_user);
  this->m_dirty = true;
}

/**
 * query users
 * @param a lambda function as the filter
 * @return a list of fitted users
 */
list<User> Storage::queryUser(function<bool(const User &)> filter) const {
  list<User> result;

  for (const User &user : this->m_userList) {
    if (filter(user)) result.push_back(user);
  }

  return result;
}

/**
 * update users
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the user
 * @return the number of updated users
 */
int Storage::updateUser(function<bool(const User &)> filter,
                        function<void(User &)> switcher) {
  int count = 0;

  for (User &user : this->m_userList) {
    if (filter(user)) {
      switcher(user);
      ++count;
    }
  }

  if (count) this->m_dirty = true;

  return count;
}

/**
 * delete users
 * @param a lambda function as the filter
 * @return the number of deleted users
 */
int Storage::deleteUser(function<bool(const User &)> filter) {
  int size = this->m_userList.size();

  this->m_userList.remove_if(filter);

  int removed = size - this->m_userList.size();

  if (removed) this->m_dirty = true;

  return removed;
}

/**
 * create a meeting
 * @param a meeting object
 */
void Storage::createMeeting(const Meeting &t_meeting) {
  this->m_meetingList.push_back(t_meeting);
  this->m_dirty = true;
}

/**
 * query meetings
 * @param a lambda function as the filter
 * @return a list of fitted meetings
 */
list<Meeting> Storage::queryMeeting(
    function<bool(const Meeting &)> filter) const {
  list<Meeting> result;

  for (const Meeting &meeting : this->m_meetingList) {
    if (filter(meeting)) result.push_back(meeting);
  }

  return result;
}

/**
 * update meetings
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the meeting
 * @return the number of updated meetings
 */
int Storage::updateMeeting(function<bool(const Meeting &)> filter,
                           function<void(Meeting &)> switcher) {
  int count = 0;

  for (Meeting &meeting : this->m_meetingList) {
    if (filter(meeting)) {
      switcher(meeting);
      ++count;
    }
  }

  if (count) this->m_dirty = true;

  return count;
}

/**
 * delete meetings
 * @param a lambda function as the filter
 * @return the number of deleted meetings
 */
int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
  int size = this->m_meetingList.size();

  this->m_meetingList.remove_if(filter);

  int removed = size - this->m_meetingList.size();

  if (removed) this->m_dirty = true;

  return removed;
}

/**
 * sync with the file
 */
bool Storage::sync(void) { return this->writeToFile(); }