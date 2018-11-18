#define GTEST
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "AgendaService.hpp"
#include "utility.h"

using std::list;
using std::string;
using std::vector;

class createCSV : public ::testing::Environment {
 public:
  /*
   *  Create users.csv and meetings.csv files
   *  This is done before storage is instantiated
   */
  virtual void SetUp() { utility::recFiles(); }
};

::testing::Environment *const env =
    ::testing::AddGlobalTestEnvironment(new createCSV);

class AgendaServiceTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() { service = new AgendaService; }
  static void TearDownTestCase() {
    //  Recover the data files
    delete service;
    service = nullptr;
    utility::recFiles();
  }
  bool meetingQuery(int user, const string &start, const string &end) {
    return !service->meetingQuery(userSamples[user].getName(), start, end)
                .empty();
  }
  void invalidMeetingIntervalTest(int i, const string &startDate,
                                  const string &endDate) {
    EXPECT_FALSE(service->createMeeting(
        meetingSamples[i].getSponsor(), meetingSamples[i].getTitle(), startDate,
        endDate, meetingSamples[i].getParticipator()))
        << "i = " << i << "\nSponsor: " << meetingSamples[i].getSponsor()
        << "\nTitle: " << meetingSamples[i].getTitle()
        << "\nstartDate: " << startDate << "\nendData: " << endDate;
  }
  void validMeetingIntervalTest(int i, const string &startDate,
                                const string &endDate) {
    EXPECT_TRUE(service->createMeeting(
        meetingSamples[i].getSponsor(), meetingSamples[i].getTitle(), startDate,
        endDate, meetingSamples[i].getParticipator()))
        << "i = " << i << "\nSponsor: " << meetingSamples[i].getSponsor()
        << "\nTitle: " << meetingSamples[i].getTitle()
        << "\nstartDate: " << startDate << "\nendData: " << endDate;
  }
  void invalidAddMeetingParticipatorTest(int i, const string &part) {
    EXPECT_FALSE(service->addMeetingParticipator(
        meetingSamples[i].getSponsor(), meetingSamples[i].getTitle(), part))
        << "Participator: " << part << "\ni = " << i
        << "\nSponsor: " << meetingSamples[i].getSponsor()
        << "\nTitle: " << meetingSamples[i].getTitle() << "\nstartDate: "
        << Date::dateToString(meetingSamples[i].getStartDate())
        << "\nendDate: " << Date::dateToString(meetingSamples[i].getEndDate());
  }
  void validAddMeetingParticipatorTest(int i, const string &part) {
    EXPECT_TRUE(service->addMeetingParticipator(
        meetingSamples[i].getSponsor(), meetingSamples[i].getTitle(), part))
        << "Participator: " << part << "\ni = " << i
        << "\nSponsor: " << meetingSamples[i].getSponsor()
        << "\nTitle: " << meetingSamples[i].getTitle() << "\nstartDate: "
        << Date::dateToString(meetingSamples[i].getStartDate())
        << "\nendDate: " << Date::dateToString(meetingSamples[i].getEndDate());
  }
  void invalidRemoveMeetingParticipatorTest(int i, const string &part) {
    EXPECT_FALSE(service->removeMeetingParticipator(
        meetingSamples[i].getSponsor(), meetingSamples[i].getTitle(), part))
        << "Participator: " << part << "\ni = " << i
        << "\nSponsor: " << meetingSamples[i].getSponsor()
        << "\nTitle: " << meetingSamples[i].getTitle() << "\nstartDate: "
        << Date::dateToString(meetingSamples[i].getStartDate())
        << "\nendDate: " << Date::dateToString(meetingSamples[i].getEndDate());
  }
  void validRemoveMeetingParticipatorTest(int i, const string &part) {
    EXPECT_TRUE(service->removeMeetingParticipator(
        meetingSamples[i].getSponsor(), meetingSamples[i].getTitle(), part))
        << "Participator: " << part << "\ni = " << i
        << "\nSponsor: " << meetingSamples[i].getSponsor()
        << "\nTitle: " << meetingSamples[i].getTitle() << "\nstartDate: "
        << Date::dateToString(meetingSamples[i].getStartDate())
        << "\nendDate: " << Date::dateToString(meetingSamples[i].getEndDate());
  }
  static AgendaService *service;
  //  user1-user3 and meeting1, meeting2 are default configuration for the test
  //  (written in data/users.csv and data/meetings.csv)
  static const User user1, user2, user3, user4;
  static const Meeting meeting1, meeting2, meeting3, meeting4, meeting5,
      meeting6;
  static const User userSamples[4];
  static const Meeting meetingSamples[6];
};

AgendaService *AgendaServiceTest::service = nullptr;

const User AgendaServiceTest::user1("Lara Croft", "TombRaidar",
                                    "lara@email.com", "13800000000");
const User AgendaServiceTest::user2("Geralt of Rivia", "TheWithcer",
                                    "geralt@email.com", "13700000000");
const User AgendaServiceTest::user3("Naked Snake", "MetalGearSolid",
                                    "snake@email.com", "13600000000");
const User AgendaServiceTest::user4("Trevor", "GrandTheftAutoV",
                                    "Trevor@email.com", "13500000000");
const Meeting AgendaServiceTest::meeting1(
    "Naked Snake", vector<string>({"Lara Croft"}),
    Date::stringToDate("2016-07-08/11:10"),
    Date::stringToDate("2016-07-08/12:05"), "I wanna Quite");
const Meeting AgendaServiceTest::meeting2(
    "Geralt of Rivia", vector<string>({"Naked Snake", "Lara Croft"}),
    Date::stringToDate("2016-07-10/15:00"),
    Date::stringToDate("2016-07-10/18:00"), "Want a few rounds of Gwent?");
const Meeting AgendaServiceTest::meeting3(
    "Lara Croft", vector<string>({"Trevor"}),
    Date::stringToDate("2017-07-11/17:21"),
    Date::stringToDate("2017-07-11/19:00"), "???");
const Meeting AgendaServiceTest::meeting4(
    "Trevor", vector<string>({"Geralt of Rivia"}),
    Date::stringToDate("2018-07-11/17:21"),
    Date::stringToDate("2018-07-11/19:00"), "????");
const Meeting AgendaServiceTest::meeting5(
    "Geralt of Rivia", vector<string>({"Naked Snake"}),
    Date::stringToDate("2019-07-11/17:21"),
    Date::stringToDate("2019-07-11/19:00"), "?????");
const Meeting AgendaServiceTest::meeting6(
    "Geralt of Rivia", vector<string>({"Trevor"}),
    Date::stringToDate("2020-07-11/17:21"),
    Date::stringToDate("2020-07-11/19:00"), "??????");
const User AgendaServiceTest::userSamples[4] = {user1, user2, user3, user4};
const Meeting AgendaServiceTest::meetingSamples[6] = {
    meeting1, meeting2, meeting3, meeting4, meeting5, meeting6};

/*
 *  Test operations on user
 */
TEST_F(AgendaServiceTest, PureUserOperations) {
  //  Login test
  EXPECT_TRUE(service->userLogIn(user1.getName(), user1.getPassword()));
  EXPECT_FALSE(service->userLogIn(user1.getName(), "password"));
  EXPECT_FALSE(service->userLogIn(user1.getName(), ""));
  EXPECT_FALSE(service->userLogIn("userName", "password"));
  EXPECT_FALSE(service->userLogIn("", ""));
  EXPECT_FALSE(service->userLogIn(user4.getName(), user4.getPassword()));
  //  Register test
  EXPECT_FALSE(service->userRegister(user3.getName(), user4.getPassword(),
                                     user4.getEmail(), user4.getPhone()));
  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  //  Get all users
  auto userList = service->listAllUsers();
  utility::testUserList(list<User>({user1, user2, user3, user4}), userList);
  //  Delete user
  EXPECT_FALSE(service->deleteUser("Trevor Philips", user4.getPassword()));
  EXPECT_FALSE(service->deleteUser(user4.getName(), user3.getPassword()));
  EXPECT_FALSE(service->deleteUser(user4.getName(), ""));
  EXPECT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));
  EXPECT_FALSE(service->userLogIn(user4.getName(), user4.getPassword()));
}

/*
 *  Test createMeeting() method
 *  not including overlap cases
 */
TEST_F(AgendaServiceTest, CreateMeeting) {
  //  Create meeiting
  //  Existed title
  EXPECT_FALSE(service->createMeeting(
      meeting5.getSponsor(), meeting2.getTitle(),
      Date::dateToString(meeting5.getStartDate()),
      Date::dateToString(meeting5.getEndDate()), meeting5.getParticipator()));
  //  No such sponsor
  EXPECT_FALSE(service->createMeeting(
      user4.getName(), meeting5.getTitle(),
      Date::dateToString(meeting5.getStartDate()),
      Date::dateToString(meeting5.getEndDate()), meeting5.getParticipator()));
  //  Some participators are dupulicate
  EXPECT_FALSE(
      service->createMeeting(meeting5.getSponsor(), meeting5.getTitle(),
                             Date::dateToString(meeting5.getStartDate()),
                             Date::dateToString(meeting5.getEndDate()),
                             vector<string>({meeting5.getParticipator()[0],
                                             meeting5.getParticipator()[0]})));
  //  Participator list contains sponsor
  EXPECT_FALSE(service->createMeeting(
      meeting5.getSponsor(), meeting5.getTitle(),
      Date::dateToString(meeting5.getStartDate()),
      Date::dateToString(meeting5.getEndDate()),
      vector<string>({meeting5.getParticipator()[0], meeting5.getSponsor()})));
  //  No such participator(s)
  EXPECT_FALSE(service->createMeeting(
      meeting3.getSponsor(), meeting3.getTitle(),
      Date::dateToString(meeting3.getStartDate()),
      Date::dateToString(meeting3.getEndDate()), meeting3.getParticipator()));
  //  Invalid start date string (format)
  EXPECT_FALSE(service->createMeeting(
      meeting5.getSponsor(), meeting5.getTitle(), "2019-7-10/15:00",
      Date::dateToString(meeting5.getEndDate()), meeting5.getParticipator()));
  //  Invalid end date string (format)
  EXPECT_FALSE(
      service->createMeeting(meeting5.getSponsor(), meeting5.getTitle(),
                             Date::dateToString(meeting5.getStartDate()),
                             "2019-07-11/19:0", meeting5.getParticipator()));
  //  Invalid start date string (logic)
  EXPECT_FALSE(service->createMeeting(
      meeting5.getSponsor(), meeting5.getTitle(), "1900-02-29/15:00",
      Date::dateToString(meeting5.getEndDate()), meeting5.getParticipator()));
  //  Invalid end date string (logic)
  EXPECT_FALSE(
      service->createMeeting(meeting5.getSponsor(), meeting5.getTitle(),
                             Date::dateToString(meeting5.getStartDate()),
                             "2100-02-29/19:00", meeting5.getParticipator()));
  //  Start date is later than end date
  EXPECT_FALSE(
      service->createMeeting(meeting5.getSponsor(), meeting5.getTitle(),
                             Date::dateToString(meeting5.getEndDate()),
                             Date::dateToString(meeting5.getStartDate()),
                             vector<string>({meeting5.getSponsor()})));
  //  Start date is equal to end date
  EXPECT_FALSE(service->createMeeting(
      meeting5.getSponsor(), meeting5.getTitle(),
      Date::dateToString(meeting5.getStartDate()),
      Date::dateToString(meeting5.getStartDate()), meeting5.getParticipator()));
}

/*
 *  Test createMeeting() method with overlaps
 */
TEST_F(AgendaServiceTest, CreateMeetingDateIssues) {
  //  Delete all meetings
  for (auto &user : service->listAllUsers()) {
    service->deleteAllMeetings(user.getName());
  }
  for (auto &user : service->listAllUsers()) {
    ASSERT_TRUE(service->listAllMeetings(user.getName()).empty());
  }
  //  Needed in the following tests
  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  ASSERT_TRUE(service->createMeeting(
      meetingSamples[4].getSponsor(), meetingSamples[4].getTitle(),
      Date::dateToString(meetingSamples[4].getStartDate()),
      Date::dateToString(meetingSamples[4].getEndDate()),
      meetingSamples[4].getParticipator()));

  //  Sponsor already sponsors a meeting
  //  Sponsor is busy in "2019-07-11/17:21" --- "2019-07-11/19:00"
  // current meeting interval           |-----|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:20", "2019-07-11/18:00");

  // current meeting interval               |---|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:21", "2019-07-11/18:00");

  // current meeting interval                |---|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:25", "2019-07-11/18:00");

  // current meeting interval                 |---|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:25", "2019-07-11/19:00");
  // current meeting interval               |-----|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:21", "2019-07-11/19:00");

  // current meeting interval                 |-----|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:25", "2019-07-11/19:01");

  // current meeting interval              |-------|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(5, "2019-07-11/17:20", "2019-07-11/19:01");

  //  Some boundary checks
  // current meeting interval         |-----|
  // sponsor's meeting interval             |-----|
  validMeetingIntervalTest(5, "2019-07-11/17:20", "2019-07-11/17:21");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[5].getSponsor(),
                                     meetingSamples[5].getTitle()));
  // current meeting interval                     |-----|
  // sponsor's meeting interval             |-----|
  validMeetingIntervalTest(5, "2019-07-11/19:00", "2019-07-11/19:05");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[5].getSponsor(),
                                     meetingSamples[5].getTitle()));

  EXPECT_TRUE(service->deleteMeeting(meetingSamples[4].getSponsor(),
                                     meetingSamples[4].getTitle()));
  //  Needed in the following test
  ASSERT_TRUE(service->createMeeting(
      meetingSamples[3].getSponsor(), meetingSamples[3].getTitle(),
      Date::dateToString(meetingSamples[3].getStartDate()),
      Date::dateToString(meetingSamples[3].getEndDate()),
      meetingSamples[3].getParticipator()));
  //  Sponsor already participates in a meeting
  //  Sponsor is busy in "2018-07-11/17:21" --- "2018-07-11/19:00"
  // current meeting interval           |-----|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:20", "2018-07-11/18:00");

  // current meeting interval               |---|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:21", "2018-07-11/18:00");

  // current meeting interval                |---|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:25", "2018-07-11/18:00");

  // current meeting interval                 |---|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:25", "2018-07-11/19:00");

  // current meeting interval               |-----|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:21", "2018-07-11/19:00");

  // current meeting interval                 |-----|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:25", "2018-07-11/19:01");

  // current meeting interval              |-------|
  // sponsor's meeting interval             |-----|
  invalidMeetingIntervalTest(4, "2018-07-11/17:20", "2018-07-11/19:01");

  //  Some boundary checks
  // current meeting interval         |-----|
  // sponsor's meeting interval             |-----|
  validMeetingIntervalTest(4, "2018-07-11/17:20", "2018-07-11/17:21");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[4].getSponsor(),
                                     meetingSamples[4].getTitle()));
  // current meeting interval                     |-----|
  // sponsor's meeting interval             |-----|
  validMeetingIntervalTest(4, "2018-07-11/19:00", "2018-07-11/19:05");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[4].getSponsor(),
                                     meetingSamples[4].getTitle()));

  EXPECT_TRUE(service->deleteMeeting(meetingSamples[3].getSponsor(),
                                     meetingSamples[3].getTitle()));

  //  Needed in the following test
  ASSERT_TRUE(service->createMeeting(
      meetingSamples[0].getSponsor(), meetingSamples[0].getTitle(),
      Date::dateToString(meetingSamples[0].getStartDate()),
      Date::dateToString(meetingSamples[0].getEndDate()),
      meetingSamples[0].getParticipator()));
  //  Some participators already sponsor meeting
  //  Participator is busy in "2016-07-08/11:10" --- "2016-07-08/12:05"
  // current meeting interval           |-----|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:00", "2016-07-08/12:00");

  // current meeting interval               |---|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:10", "2016-07-08/12:00");

  // current meeting interval                |---|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:25", "2016-07-08/12:00");

  // current meeting interval                 |---|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:25", "2016-07-08/12:05");

  // current meeting interval               |-----|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:10", "2016-07-08/12:05");

  // current meeting interval                 |-----|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:25", "2016-07-08/12:10");

  // current meeting interval              |-------|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(4, "2016-07-08/11:00", "2016-07-08/12:10");

  //  Some boundary checks
  // current meeting interval         |-----|
  // participator's meeting interval        |-----|
  validMeetingIntervalTest(4, "2016-07-08/11:00", "2016-07-08/11:10");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[4].getSponsor(),
                                     meetingSamples[4].getTitle()));
  // current meeting interval                     |-----|
  // participator's meeting interval        |-----|
  validMeetingIntervalTest(4, "2016-07-08/12:05", "2016-07-08/12:10");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[4].getSponsor(),
                                     meetingSamples[4].getTitle()));

  EXPECT_TRUE(service->deleteMeeting(meetingSamples[0].getSponsor(),
                                     meetingSamples[0].getTitle()));

  //  Needed in the following test
  ASSERT_TRUE(service->createMeeting(
      meetingSamples[5].getSponsor(), meetingSamples[5].getTitle(),
      Date::dateToString(meetingSamples[5].getStartDate()),
      Date::dateToString(meetingSamples[5].getEndDate()),
      meetingSamples[5].getParticipator()));

  //  Some participators already participate in meeting
  //  Participator is busy in "2020-07-11/17:21" --- "2020-07-11/19:00"
  // current meeting interval           |-----|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:00", "2020-07-11/18:00");

  // current meeting interval               |---|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:21", "2020-07-11/18:00");

  // current meeting interval                |---|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:25", "2020-07-11/18:00");

  // current meeting interval                 |---|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:25", "2020-07-11/19:00");

  // current meeting interval               |-----|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:21", "2020-07-11/19:00");

  // current meeting interval                 |-----|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:25", "2020-07-11/19:10");

  // current meeting interval              |-------|
  // participator's meeting interval        |-----|
  invalidMeetingIntervalTest(2, "2020-07-11/17:00", "2020-07-11/19:10");

  //  Some boundary checks
  // current meeting interval         |-----|
  // participator's meeting interval        |-----|
  validMeetingIntervalTest(2, "2020-07-11/11:00", "2020-07-11/17:21");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[2].getSponsor(),
                                     meetingSamples[2].getTitle()));
  // current meeting interval                     |-----|
  // participator's meeting interval        |-----|
  validMeetingIntervalTest(2, "2020-07-11/19:00", "2020-07-11/20:10");
  EXPECT_TRUE(service->deleteMeeting(meetingSamples[2].getSponsor(),
                                     meetingSamples[2].getTitle()));

  EXPECT_TRUE(service->deleteMeeting(meetingSamples[5].getSponsor(),
                                     meetingSamples[5].getTitle()));

  EXPECT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));
  for (auto &user : service->listAllUsers()) {
    ASSERT_TRUE(service->listAllMeetings(user.getName()).empty());
  }
}

/*
 *  Test addMeetingParticipator() and removeMeetingParticipator()
 */
TEST_F(AgendaServiceTest, addAndRemoveMeetingParticipator) {
  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  ASSERT_TRUE(service->createMeeting(
      meeting1.getSponsor(), meeting1.getTitle(),
      Date::dateToString(meeting1.getStartDate()),
      Date::dateToString(meeting1.getEndDate()), meeting1.getParticipator()));

  // no such meeting
  invalidAddMeetingParticipatorTest(3, "Trevor");
  invalidRemoveMeetingParticipatorTest(3, "Trevor");

  // test add sponsor
  invalidAddMeetingParticipatorTest(0, "Naked Snake");

  validAddMeetingParticipatorTest(0, "Trevor");

  // test add again
  invalidAddMeetingParticipatorTest(0, "Trevor");
  // no such user
  invalidAddMeetingParticipatorTest(0, "LLLL");

  // remove the participator
  validRemoveMeetingParticipatorTest(0, "Trevor");
  // test remove again
  invalidRemoveMeetingParticipatorTest(0, "Trevor");
  // check if already removed
  auto meetings = service->listAllMeetings("Trevor");
  EXPECT_TRUE(meetings.empty());

  // remove the last participator
  validRemoveMeetingParticipatorTest(0, "Lara Croft");
  // check if the meeting already deleted
  meetings = service->listAllMeetings("Naked Snake");
  ASSERT_TRUE(meetings.empty());

  invalidRemoveMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));
  // no need to delete because when the last participator is removed, the
  // meeting will be deleted

  // ASSERT_TRUE(service->deleteMeeting(meeting1.getSponsor(),
  // meeting1.getTitle()));
}

/*
 *  Test deleteUser side effect
 */
TEST_F(AgendaServiceTest, deleteUserSideEffect) {
  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  ASSERT_TRUE(service->createMeeting(
      meeting1.getSponsor(), meeting1.getTitle(),
      Date::dateToString(meeting1.getStartDate()),
      Date::dateToString(meeting1.getEndDate()), meeting1.getParticipator()));

  validAddMeetingParticipatorTest(0, "Trevor");
  auto meeting =
      service->meetingQuery(meeting1.getSponsor(), meeting1.getTitle()).front();
  // 2 participators. Trevor and Lara
  EXPECT_TRUE(meeting.getParticipator().size() == 2);

  ASSERT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));
  meeting =
      service->meetingQuery(meeting1.getSponsor(), meeting1.getTitle()).front();
  // side effect -- remove participator. Trevor will be removed
  EXPECT_TRUE(meeting.getParticipator().size() == 1);

  // register again
  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  // add again
  validAddMeetingParticipatorTest(0, "Trevor");
  validRemoveMeetingParticipatorTest(0, "Lara Croft");
  ASSERT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));
  // side effect -- removing participator results in meeting being deleted.
  // Meeting will be deleted
  ASSERT_TRUE(service->meetingQuery(meeting1.getSponsor(), meeting1.getTitle())
                  .empty());

  // register again
  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  ASSERT_TRUE(service->createMeeting(
      meeting4.getSponsor(), meeting4.getTitle(),
      Date::dateToString(meeting4.getStartDate()),
      Date::dateToString(meeting4.getEndDate()), meeting4.getParticipator()));
  // side effect -- deleting sponsor results in meeting being deleted. Meeting
  // will be deleted
  ASSERT_FALSE(service->meetingQuery(meeting4.getSponsor(), meeting4.getTitle())
                   .empty());
  ASSERT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));
  ASSERT_TRUE(service->meetingQuery(meeting4.getSponsor(), meeting4.getTitle())
                  .empty());
}

/*
 *  Test addMeetingParticipator() Date Issues
 */
TEST_F(AgendaServiceTest, addMeetingParticipatorDateIssues) {
  ASSERT_TRUE(service->createMeeting(
      meeting1.getSponsor(), meeting1.getTitle(),
      Date::dateToString(meeting1.getStartDate()),
      Date::dateToString(meeting1.getEndDate()), meeting1.getParticipator()));

  ASSERT_TRUE(service->userRegister(user4.getName(), user4.getPassword(),
                                    user4.getEmail(), user4.getPhone()));
  // Participator (Trevor) is busy
  // participator's current interval    |-----|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:00", "2016-07-08/12:00",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval        |---|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:10", "2016-07-08/12:00",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval         |---|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:25", "2016-07-08/12:00",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval          |---|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:25", "2016-07-08/12:05",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval        |-----|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:10", "2016-07-08/12:05",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval          |-----|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:25", "2016-07-08/12:10",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval       |-------|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:00", "2016-07-08/12:10",
                                     meeting4.getParticipator()));
  invalidAddMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval  |-----|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/11:00", "2016-07-08/11:10",
                                     meeting4.getParticipator()));
  validAddMeetingParticipatorTest(0, "Trevor");
  validRemoveMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  // participator's current interval              |-----|
  // participator's to add interval         |-----|
  ASSERT_TRUE(service->createMeeting(meeting4.getSponsor(), meeting4.getTitle(),
                                     "2016-07-08/12:05", "2016-07-08/12:10",
                                     meeting4.getParticipator()));
  validAddMeetingParticipatorTest(0, "Trevor");
  validRemoveMeetingParticipatorTest(0, "Trevor");
  ASSERT_TRUE(
      service->deleteMeeting(meeting4.getSponsor(), meeting4.getTitle()));

  ASSERT_TRUE(service->deleteUser(user4.getName(), user4.getPassword()));

  ASSERT_TRUE(
      service->deleteMeeting(meeting1.getSponsor(), meeting1.getTitle()));
}

/*
 *  Test quitMeeting() method
 */
TEST_F(AgendaServiceTest, QuitMeeting) {
  ASSERT_TRUE(service->createMeeting(
      meeting2.getSponsor(), meeting2.getTitle(),
      Date::dateToString(meeting2.getStartDate()),
      Date::dateToString(meeting2.getEndDate()), meeting2.getParticipator()));

  auto meetings = service->listAllMeetings("Lara Croft");
  ASSERT_FALSE(meetings.empty());

  // no such meeting
  EXPECT_FALSE(service->quitMeeting("Trevor", "Test"));

  // as the sponsor, sponsor can not quit the meeting
  EXPECT_FALSE(
      service->quitMeeting(meeting2.getSponsor(), meeting2.getTitle()));

  // no such participator
  EXPECT_FALSE(service->quitMeeting("Trevor", meeting2.getTitle()));

  // success
  EXPECT_TRUE(service->quitMeeting("Lara Croft", meeting2.getTitle()));
  meetings = service->listAllMeetings("Lara Croft");
  EXPECT_TRUE(meetings.empty());

  // success
  EXPECT_TRUE(service->quitMeeting("Naked Snake", meeting2.getTitle()));
  // no participator, should be deleted
  meetings = service->listAllMeetings("Geralt of Rivia");
  ASSERT_TRUE(meetings.empty());

  // already deleted here
  // ASSERT_TRUE(service->deleteMeeting(meeting2.getSponsor(),
  // meeting2.getTitle()));
}

/*
 *  Test meetingQuery() method
 */
TEST_F(AgendaServiceTest, MeetingQuery) {
  service->userRegister(user4.getName(), user4.getPassword(), user4.getEmail(),
                        user4.getPhone());
  ASSERT_TRUE(service->userLogIn(user4.getName(), user4.getPassword()));
  for (auto &meeting : meetingSamples) {
    //  Query with sponsor's name and title
    EXPECT_TRUE(service->meetingQuery(meeting.getSponsor(), meeting.getTitle())
                    .empty());
  }
  for (auto &meeting : meetingSamples) {
    ASSERT_TRUE(service->createMeeting(
        meeting.getSponsor(), meeting.getTitle(),
        Date::dateToString(meeting.getStartDate()),
        Date::dateToString(meeting.getEndDate()), meeting.getParticipator()));
  }
  for (auto &meeting : meetingSamples) {
    //  Query with sponsor's name and title
    ASSERT_FALSE(service->meetingQuery(meeting.getSponsor(), meeting.getTitle())
                     .empty());
  }

  //  Invalid start date string (format)
  EXPECT_TRUE(service
                  ->meetingQuery(meeting5.getSponsor(), "2019-7-10/15:00",
                                 "2019-07-10/19:00")
                  .empty());
  //  Invalid end date string (format)
  EXPECT_TRUE(service
                  ->meetingQuery(meeting5.getSponsor(), "2019-07-10/15:00",
                                 "2019-7-10/19:00")
                  .empty());
  //  Invalid start date string (logic)
  EXPECT_TRUE(service
                  ->meetingQuery(meeting5.getSponsor(), "1900-02-29/15:00",
                                 "2019-07-10/15:00")
                  .empty());
  //  Invalid end date string (logic)
  EXPECT_TRUE(service
                  ->meetingQuery(meeting5.getSponsor(), "2019-07-10/15:00",
                                 "2100-02-29/15:00")
                  .empty());
  //  Query with user's name and time interval
  //  Current users
  //  0. Lara 1. Geralt 2.Snake 3. Trevor
  //  Current meetings
  //  0. Sponsor: "Naked Snake",     Participator: "Lara Croft", StartDate:
  //  "2016-07-08/11:10", EndDate: "2016-07-08/12:05", Title: "I wanna Quite"
  //  1. Sponsor: "Geralt of Rivia", Participator: "Naked Snake", "Lara Croft",
  //  StartDate: "2016-07-10/15:00", EndDate: "2016-07-10/18:00", Title: "Want a
  //  few rounds of Gwent?"
  //  2. Sponsor: "Lara Croft",      Participator: "Trevor", StartDate:
  //  "2017-07-11/17:21", EndDate: "2017-07-11/19:00", Title: "???"
  //  3. Sponsor: "Trevor",          Participator: "Geralt of Rivia", StartDate:
  //  "2018-07-11/17:21", EndDate: "2018-07-11/19:00", Title: "????"
  //  4. Sponsor: "Geralt of Rivia", Participator: "Naked Snake", StartDate:
  //  "2019-07-11/17:21", EndDate: "2019-07-11/19:00", Title: "?????"
  //  5. Sponsor: "Geralt of Rivia", Participator: "Trevor", StartDate:
  //  "2020-07-11/17:21", EndDate: "2020-07-11/19:00", Title: "??????"

  //  Both busy intervals are as participators
  //  Busy time: "2016-07-08/11:10" --- "2016-07-08/12:05" "2016-07-10/15:00"
  //  --- "2016-07-10/18:00" Query interval             |-0-| User busy interval
  //  |---|       |---|
  EXPECT_FALSE(meetingQuery(0, "2016-07-08/13:00", "2016-07-10/14:00"));
  //  Query interval          |-0-|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/11:00", "2016-07-10/14:00"));
  //  Query interval                |-0-|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/13:00", "2016-07-10/16:00"));
  //  Query interval           |---0--|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/12:05", "2016-07-10/14:00"));
  //  Query interval            |---0--|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/13:05", "2016-07-10/15:00"));
  //  Query interval          |----0----|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/12:00", "2016-07-10/16:00"));
  //  Query interval      |--------0--------|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/11:00", "2016-07-10/19:00"));
  //  Query interval      |--0--|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/11:00", "2016-07-08/16:00"));
  //  Query interval          |------0------|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/12:00", "2016-07-10/19:00"));
  //  Query interval      |-----0-------|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(0, "2016-07-08/12:00", "2016-07-10/16:00"));

  //  One of the interval is as sponsor
  //  Busy time: "2018-07-11/17:21" --- "2018-07-11/19:00" "2019-07-11/17:21"
  //  --- "2019-07-11/19:00" Query interval             |-1-| User busy interval
  //  |---|       |---|
  EXPECT_FALSE(meetingQuery(1, "2018-07-11/20:00", "2019-07-10/14:00"));
  //  Query interval          |-1-|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/17:25", "2019-07-11/18:00"));
  //  Query interval                |-1-|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/20:00", "2019-07-11/18:00"));
  //  Query interval           |---1--|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/19:00", "2018-07-11/20:00"));
  //  Query interval            |---1--|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/20:05", "2019-07-11/17:21"));
  //  Query interval          |----1----|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/17:00", "2019-07-11/18:00"));
  //  Query interval      |--------1--------|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/11:00", "2019-07-11/20:00"));
  //  Query interval      |--1--|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/17:00", "2018-07-11/20:00"));
  //  Query interval          |------1------|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/18:00", "2019-07-11/20:00"));
  //  Query interval      |-----1-------|
  //  User busy interval   |---|       |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-08/12:00", "2019-07-11/18:00"));

  //  Query a meeting at a moment
  //  Query interval         |
  //  User busy interval   |---|
  EXPECT_TRUE(meetingQuery(1, "2018-07-11/17:30", "2018-07-11/17:30"));
  //  Query interval             |
  //  User busy interval   |---|
  EXPECT_FALSE(meetingQuery(1, "2018-07-11/20:10", "2018-07-11/20:10"));

  //  End date is earlier than start date
  EXPECT_FALSE(meetingQuery(1, "2019-07-11/18:00", "2019-07-11/17:00"));
}

/*
 *  Test list meetings sponsored by a user or participated by a user
 *  Test deleteUser() and query meetings
 */
TEST_F(AgendaServiceTest, ListMeeting) {
  EXPECT_TRUE(utility::testMeetingList(
      list<Meeting>({meeting2, meeting4, meeting5, meeting6}),
      service->listAllMeetings(user2.getName())));
  EXPECT_TRUE(utility::testMeetingList(
      list<Meeting>({meeting2, meeting5, meeting6}),
      service->listAllSponsorMeetings(user2.getName())));
  EXPECT_TRUE(utility::testMeetingList(
      list<Meeting>({meeting4}),
      service->listAllParticipateMeetings(user2.getName())));
  EXPECT_TRUE(service->listAllMeetings("Trevor Philips").empty());

  ASSERT_TRUE(service->deleteUser(user2.getName(), user2.getPassword()));
  EXPECT_TRUE(service->listAllMeetings(user2.getName()).empty());
  EXPECT_TRUE(service->listAllSponsorMeetings(user2.getName()).empty());
  EXPECT_TRUE(service->listAllParticipateMeetings(user2.getName()).empty());
}
