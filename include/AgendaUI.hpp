#ifndef AGENDA_UI_HPP_
#define AGENDA_UI_HPP_

#include <iostream>
#include <string>
#include "AgendaService.hpp"
#include "Logger.hpp"

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop();

  /**
   * quit the Agenda
   */
  void quitAgenda(int error);

 private:
  /**
   * constructor
   */
  void startAgenda();

  /**
   * catch user's operation
   * @return the operation
   */
  std::string getOperation();

  /**
   * execute the operation
   * @return if the operationloop continue
   */
  bool executeOperation(std::string t_operation);

  /**
   * user Login
   */
  void userLogIn();

  /**
   * user regist
   */
  void userRegister();

  /**
   * user logout
   */
  void userLogOut();

  /**
   * delete a user from storage
   */
  void deleteUser();

  /**
   * list all users from storage
   */
  void listAllUsers();

  /**
   * user create a meeting with someone else
   */
  void createMeeting();

  /**
   * add a participator to a meeting
   */
  void addMeetingParticipator();

  /**
   * remove a participator from a meeting
   */
  void removeMeetingParticipator();

  /**
   * quit from a meeting
   */
  void quitMeeting();

  /**
   * list all meetings from storage
   */
  void listAllMeetings();

  /**
   * list all meetings that this user sponsored
   */
  void listAllSponsorMeetings();

  /**
   * list all meetings that this user take part in
   */
  void listAllParticipateMeetings();

  /**
   * search meetings by title from storage
   */
  void queryMeetingByTitle();

  /**
   * search meetings by timeinterval from storage
   */
  void queryMeetingByTimeInterval();

  /**
   * delete meetings by title from storage
   */
  void deleteMeetingByTitle();

  /**
   * delete all meetings that this user sponsored
   */
  void deleteAllMeetings();

  /**
   * show the meetings in the s creen
   */
  void printMeetings(std::list<Meeting> t_meetings);
  // dates
  std::string m_userName;
  std::string m_userPassword;
  AgendaService m_agendaService;
  std::shared_ptr<Logger> m_logger;
};

#endif
