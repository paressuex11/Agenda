#include "AgendaUI.hpp"
#include <algorithm>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Exception.hpp"

#define IS_LOG_IN (this->m_userName != "")

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::setw;
using std::string;
using std::to_string;
using std::vector;

void printManual(bool isLoggedIn) {
  for (int i = 1; i <= 37; i++) cout << "-";
  cout << "Agenda";
  for (int i = 1; i <= 37; i++) cout << "-";
  cout << endl;
  cout << "Action :" << endl;

  if (!isLoggedIn) {
    cout << "l    - log in Agenda by user name and password" << endl;
    cout << "r    - register an Agenda account" << endl;
    cout << "q    - quit Agenda" << endl;
  } else {
    cout << "o    - log out Agenda" << endl;
    cout << "dc   - delete Agenda account" << endl;
    cout << "lu   - list all Agenda user" << endl;
    cout << "cm   - create a meeting" << endl;
    cout << "amp  - add a participator to a meeting" << endl;
    cout << "rmp  - remove a participator from a meeting" << endl;
    cout << "rqm  - quit meeting" << endl;
    cout << "la   - list all meetings" << endl;
    cout << "las  - list all sponsor meetings" << endl;
    cout << "lap  - list all participator meetings" << endl;
    cout << "qm   - query meeting by title" << endl;
    cout << "qt   - query meeting by time interval" << endl;
    cout << "dm   - delete meeting by title" << endl;
    cout << "da   - delete all meetings" << endl;
  }

  for (int i = 1; i <= 80; i++) cout << "-";
  cout << endl << endl;
}

std::ostream &printPrompt(const string &data) {
  cout << "[" + data + "] ";
  return cout;
}

void printFormat(std::initializer_list<string> format) {
  for (const auto &item : format) {
    printPrompt(item);
  }

  cout << endl;
}

string turnVectorToString(const std::vector<string> &participators) {
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

int tokenize(const string &line, vector<string> &dest) {
  if (line == "") return 0;

  vector<string> result;
  int origin_size = dest.size();

  for (string::size_type start = 0;;) {
    string::size_type bound = line.find(' ', start);

    if (bound == string::npos) {
      dest.push_back(line.substr(start));
      break;
    }

    dest.push_back(line.substr(start, bound - start));

    for (start = bound; start < line.size() and line[start] == ' '; ++start)
      ;

    if (start == line.size()) break;
  }

  return dest.size() - origin_size;
}

string getStrippedLine() {
  string str;

  getline(cin, str);

  string::size_type head(0);
  string::size_type tail(str.size());

  while (head < str.size() && isspace(str[head])) head++;
  while (tail > 0 && isspace(str[tail - 1])) tail++;

  return str.substr(head, tail - head);
}

vector<string> getArguments(const int argNum, string prompt) {
  vector<string> result;
  int argLeft = argNum;

  for (string line; argLeft;) {
    printPrompt(prompt);
    line = getStrippedLine();

    int argRcv = tokenize(line, result);

    if (argRcv > argLeft) {
      throw wrong_arg_num(to_string(argLeft) + " args left, while receive " +
                          to_string(argRcv));
    } else {
      argLeft -= argRcv;
    }
  }

  return result;
}

void checkLoginState(bool requiredLogin, bool actualLogin,
                     const string &action) {
  if (requiredLogin != actualLogin) {
    if (!requiredLogin) {
      throw permission_error("Command (" + action +
                             ") require a 'Log Out' state.");
    } else {
      throw permission_error("Command (" + action +
                             ") require a 'Log In' state.");
    }
  }
}

AgendaUI::AgendaUI() {
  this->m_userName = "";
  this->m_userPassword = "";
  this->startAgenda();
}

void AgendaUI::OperationLoop() {
  while (true) {
    try {
      string operation = this->getOperation();

      for (int i = 0; i < operation.length(); i++)
        operation[i] = tolower(operation[i]);

      if (!this->executeOperation(operation)) break;
    } catch (const my_exception &e) {
      cerr << endl << '[' << e.type() << "] " << e.what() << endl << endl;
    }
  }
}

/**
 * constructor
 */
void AgendaUI::startAgenda() {
  system("clear");
  printManual(false);
}

/**
 * catch user's operation
 * @return the operation
 */
string AgendaUI::getOperation() {
  string input;

  cout << "Agenda";

  if (IS_LOG_IN) {
    cout << "@" << this->m_userName << " :~# ";
  } else {
    cout << " :~$ ";
  }

  return getStrippedLine();
}

/**
 * execute the operation
 * @return if the operationloop continue
 */
bool AgendaUI::executeOperation(string t_operation) {
  if (t_operation == "") return true;

  if (t_operation == "l") {
    checkLoginState(false, IS_LOG_IN, t_operation);
    this->userLogIn();
  } else if (t_operation == "r") {
    checkLoginState(false, IS_LOG_IN, t_operation);
    this->userRegister();
  } else if (t_operation == "q") {
    this->quitAgenda();
  } else if (t_operation == "o") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->userLogOut();
  } else if (t_operation == "dc") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->deleteUser();
  } else if (t_operation == "lu") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->listAllUsers();
  } else if (t_operation == "cm") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->createMeeting();
  } else if (t_operation == "amp") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->addMeetingParticipator();
  } else if (t_operation == "rmp") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->removeMeetingParticipator();
  } else if (t_operation == "rqm") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->quitMeeting();
  } else if (t_operation == "la") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->listAllMeetings();
  } else if (t_operation == "las") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->listAllSponsorMeetings();
  } else if (t_operation == "lap") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->listAllParticipateMeetings();
  } else if (t_operation == "qm") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->queryMeetingByTitle();
  } else if (t_operation == "qt") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->queryMeetingByTimeInterval();
  } else if (t_operation == "dm") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->deleteMeetingByTitle();
  } else if (t_operation == "da") {
    checkLoginState(true, IS_LOG_IN, t_operation);
    this->deleteAllMeetings();
  } else if (t_operation == "h") {
    cout << endl;
    printManual(IS_LOG_IN);
  } else {
    throw unknown_command(t_operation);
  }

  return true;
}

/**
 * user Login
 */
void AgendaUI::userLogIn() {
  string prompt = "log in";

  cout << endl;
  printPrompt(prompt);
  printFormat({"username", "password"});

  vector<string> arguments = getArguments(2, prompt);
  bool result = this->m_agendaService.userLogIn(arguments[0], arguments[1]);

  if (result) {
    this->m_userName = arguments[0];
    this->m_userPassword = arguments[1];
    printPrompt(prompt) << "succeed!" << endl << endl;
    printManual(true);
  } else {
    printPrompt(prompt) << "Password error or user doesn't exist" << endl
                        << endl;
  }
}

/**
 * user regist
 */
void AgendaUI::userRegister() {
  string prompt = "register";

  cout << endl;
  printPrompt(prompt);
  printFormat({"username", "password", "email", "phone"});

  vector<string> arguments = getArguments(4, prompt);

  bool result = this->m_agendaService.userRegister(arguments[0], arguments[1],
                                                   arguments[2], arguments[3]);

  if (result) {
    this->m_userName = arguments[0];
    this->m_userPassword = arguments[1];
    printPrompt(prompt) << "succeed!" << endl << endl;
    printManual(true);
  } else {
    printPrompt(prompt) << "This username has been registered!" << endl << endl;
  }
}

/**
 * user logout
 */
void AgendaUI::userLogOut() {
  this->m_userName = "";
  this->m_userPassword = "";
  cout << endl;
  printManual(false);
}

/**
 * quit the Agenda
 */
void AgendaUI::quitAgenda() { exit(0); }

/**
 * delete a user from storage
 */
void AgendaUI::deleteUser() {
  string prompt = "delete agenda account";
  bool result =
      this->m_agendaService.deleteUser(this->m_userName, this->m_userPassword);

  if (result) {
    this->m_userName = "";
    this->m_userPassword = "";
    cout << endl;
    printPrompt(prompt) << "succeed!" << endl << endl;
  } else {
    printPrompt(prompt) << "error!" << endl << endl;
  }
}

/**
 * list all users from storage
 */
void AgendaUI::listAllUsers(void) {
  cout << endl;
  printPrompt("list all users") << endl << endl;
  cout << std::left << setw(20) << "name " << setw(30) << "email "
       << "phone" << endl;

  list<User> userList = this->m_agendaService.listAllUsers();

  for (const auto &user : userList) {
    cout << setw(20) << user.getName() << setw(30) << user.getEmail()
         << user.getPhone() << endl;
  }

  cout << endl;
}

/**
 * user create a meeting with someone else
 */
void AgendaUI::createMeeting() {
  string prompt = "create meeting";

  cout << endl;
  printPrompt(prompt);
  printFormat({"the number of participators"});
  printPrompt(prompt);

  string participatorStr = getStrippedLine();

  if (participatorStr == "" ||
      !std::all_of(participatorStr.begin(), participatorStr.end(), isdigit)) {
    printPrompt(prompt) << "the number of participators must be an integer!"
                        << endl
                        << endl;
    cin.clear();
    return;
  }

  int participatorNum = std::stoi(participatorStr);

  if (participatorNum == 0) {
    printPrompt(prompt) << "the number of participators can't be zero!" << endl
                        << endl;
    return;
  }

  vector<string> participators;

  for (int i = 0; i < participatorNum;) {
    string format = "please enter the participator " + std::to_string(i + 1);

    printPrompt(prompt);
    printFormat({format});
    printPrompt(prompt);

    string participator = getStrippedLine();

    if (participator == "") continue;

    participators.push_back(participator);
    i++;
  }

  printPrompt(prompt);
  printFormat(
      {"title", "start time(yyyy-mm-dd/hh:mm)", "end time(yyyy-mm-dd/hh:mm)"});

  vector<string> arguments = getArguments(3, prompt);
  bool result = this->m_agendaService.createMeeting(
      this->m_userName, arguments[0], arguments[1], arguments[2],
      participators);

  if (result) {
    printPrompt(prompt) << "succeed!" << endl << endl;
  } else {
    printPrompt(prompt) << "error!" << endl << endl;
  }
}

/**
 * add a participator to a meeting
 */
void AgendaUI::addMeetingParticipator() {
  string prompt = "add participator";

  cout << endl;
  printPrompt(prompt);
  printFormat({"meeting title", "participator username"});

  vector<string> arguments = getArguments(2, prompt);
  bool result = this->m_agendaService.addMeetingParticipator(
      this->m_userName, arguments[0], arguments[1]);

  if (result) {
    printPrompt(prompt) << "succeed!" << endl << endl;
  } else {
    printPrompt(prompt) << "error!" << endl << endl;
  }
}

/**
 * remove a participator from a meeting
 */
void AgendaUI::removeMeetingParticipator() {
  string prompt = "remove participator";

  cout << endl;
  printPrompt(prompt);
  printFormat({"meeting title", "participator username"});

  vector<string> arguments = getArguments(2, prompt);
  bool result = this->m_agendaService.removeMeetingParticipator(
      this->m_userName, arguments[0], arguments[1]);

  if (result) {
    printPrompt(prompt) << "succeed!" << endl << endl;
  } else {
    printPrompt(prompt) << "error!" << endl << endl;
  }
}

/**
 * quit from a meeting
 */
void AgendaUI::quitMeeting() {
  string prompt = "quit meeting";

  cout << endl;
  printPrompt(prompt);
  printFormat({"meeting title"});

  vector<string> arguments = getArguments(1, prompt);
  bool result =
      this->m_agendaService.quitMeeting(this->m_userName, arguments[0]);

  if (result) {
    printPrompt(prompt) << "succeed!" << endl << endl;
  } else {
    printPrompt(prompt) << "error!" << endl << endl;
  }
}

/**
 * list all meetings from storage
 */
void AgendaUI::listAllMeetings() {
  cout << endl;
  printPrompt("list all meetings") << endl << endl;

  list<Meeting> meetingList =
      this->m_agendaService.listAllMeetings(this->m_userName);

  if (meetingList.empty()) {
    cout << "None" << endl << endl;
    return;
  }

  this->printMeetings(meetingList);
}

/**
 * list all meetings that this user sponsored
 */
void AgendaUI::listAllSponsorMeetings() {
  cout << endl;
  printPrompt("list all sponsor meetings") << endl << endl;

  list<Meeting> meetingList =
      this->m_agendaService.listAllSponsorMeetings(this->m_userName);

  if (meetingList.empty()) {
    cout << "None" << endl << endl;
    return;
  }

  this->printMeetings(meetingList);
}

/**
 * list all meetings that this user take part in
 */
void AgendaUI::listAllParticipateMeetings() {
  cout << endl;
  printPrompt("list all participator meetings") << endl << endl;

  list<Meeting> meetingList =
      this->m_agendaService.listAllParticipateMeetings(this->m_userName);

  if (meetingList.empty()) {
    cout << "None" << endl << endl;
    return;
  }

  this->printMeetings(meetingList);
}

/**
 * search meetings by title from storage
 */
void AgendaUI::queryMeetingByTitle() {
  string prompt = "query meeting";

  cout << endl;
  printPrompt(prompt);
  printFormat({"title"});

  vector<string> arguments = getArguments(1, prompt);

  list<Meeting> meetingList =
      this->m_agendaService.meetingQuery(this->m_userName, arguments[0]);

  if (meetingList.empty()) {
    cout << "None" << endl << endl;
    return;
  }

  cout << endl;
  this->printMeetings(meetingList);
}

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::queryMeetingByTimeInterval() {
  string prompt = "query meetings";

  cout << endl;
  printPrompt(prompt);
  printFormat({"start time(yyyy-mm-dd/hh:mm)", "end time(yyyy-mm-dd/hh:mm)"});

  vector<string> arguments = getArguments(2, prompt);

  list<Meeting> meetingList = this->m_agendaService.meetingQuery(
      this->m_userName, arguments[0], arguments[1]);

  if (meetingList.empty()) {
    cout << "None" << endl << endl;
    return;
  }

  cout << endl;
  this->printMeetings(meetingList);
}

/**
 * delete meetings by title from storage
 */
void AgendaUI::deleteMeetingByTitle() {
  string prompt = "delete meeting";

  cout << endl;
  printPrompt(prompt);
  printFormat({"title"});

  vector<string> arguments = getArguments(1, prompt);

  bool result =
      this->m_agendaService.deleteMeeting(this->m_userName, arguments[0]);

  if (result) {
    printPrompt(prompt) << "succeed!" << endl << endl;
  } else {
    printPrompt(prompt) << "error!" << endl << endl;
  }
}

/**
 * delete all meetings that this user sponsored
 */
void AgendaUI::deleteAllMeetings() {
  cout << endl;
  this->m_agendaService.deleteAllMeetings(this->m_userName);
  printPrompt("delete all meetings") << "succeed!" << endl << endl;
}

/**
 * show the meetings in the screen
 */
void AgendaUI::printMeetings(list<Meeting> t_meetings) {
  cout << std::left << setw(15) << "title" << setw(15) << "sponsor" << setw(18)
       << "start time" << setw(18) << "end time"
       << "participators" << endl;

  for (const auto &meeting : t_meetings) {
    cout << setw(15) << meeting.getTitle();
    cout << setw(15) << meeting.getSponsor();
    cout << setw(18) << Date::dateToString(meeting.getStartDate());
    cout << setw(18) << Date::dateToString(meeting.getEndDate());
    cout << turnVectorToString(meeting.getParticipator());
    cout << endl;
  }

  cout << endl;
}