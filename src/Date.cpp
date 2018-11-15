#include "Date.hpp"
#include <regex>
#include <vector>

using std::stoi;
using std::string;
using std::to_string;
using std::vector;

const int days_of_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @brief check whether the year is leap
 * @param a integer indicate the year to check
 * @return the bool indicate leap or not
 */
bool isLeap(int t_year) {
  return (t_year % 4 == 0 && t_year % 100 != 0 || t_year % 400 == 0);
}

/**
 * @brief check whether the year is leap
 * @param a integer indicate the integer to format
 * @return the formatted string
 */
string formatString(const int t_num) {
  string result = to_string(t_num);

  if (t_num < 10) result = '0' + result;

  return result;
}

/**
 * @brief default constructor
 */
Date::Date() : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {}

/**
 * @brief constructor with arguments
 */
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
    : m_year(t_year),
      m_month(t_month),
      m_day(t_day),
      m_hour(t_hour),
      m_minute(t_minute) {}

/**
 * @brief constructor with a string
 */
Date::Date(const string &dateString) : Date() {
  *this = stringToDate(dateString);
}

/**
 * @brief return the year of a Date
 * @return   a integer indicate the year of a date
 */
int Date::getYear(void) const { return this->m_year; }

/**
 * @brief set the year of a date
 * @param a integer indicate the new year of a date
 */
void Date::setYear(const int t_year) { this->m_year = t_year; }

/**
 * @brief return the month of a Date
 * @return   a integer indicate the month of a date
 */
int Date::getMonth(void) const { return this->m_month; }

/**
 * @brief set the month of a date
 * @param a integer indicate the new month of a date
 */
void Date::setMonth(const int t_month) { this->m_month = t_month; }

/**
 * @brief return the day of a Date
 * @return   a integer indicate the day of a date
 */
int Date::getDay(void) const { return this->m_day; }

/**
 * @brief set the day of a date
 * @param a integer indicate the new day of a date
 */
void Date::setDay(const int t_day) { this->m_day = t_day; }

/**
 * @brief return the hour of a Date
 * @return   a integer indicate the hour of a date
 */
int Date::getHour(void) const { return this->m_hour; }

/**
 * @brief set the hour of a date
 * @param a integer indicate the new hour of a date
 */
void Date::setHour(const int t_hour) { this->m_hour = t_hour; }

/**
 * @brief return the minute of a Date
 * @return   a integer indicate the minute of a date
 */
int Date::getMinute(void) const { return this->m_minute; }

/**
 * @brief set the minute of a date
 * @param a integer indicate the new minute of a date
 */
void Date::setMinute(const int t_minute) { m_minute = t_minute; }

/**
 *   @brief check whether the date is valid or not
 *   @return the bool indicate valid or not
 */
bool Date::isValid(const Date &t_date) {
  if (t_date.m_hour < 0 || t_date.m_hour >= 24) return false;

  if (t_date.m_minute < 0 || t_date.m_minute >= 60) return false;

  if (t_date.m_year < 1000 || t_date.m_year >= 10000) return false;

  if (t_date.m_month <= 0 || t_date.m_month > 12) return false;

  if (t_date.m_day <= 0) return false;

  if (isLeap(t_date.m_year) && t_date.m_month == 2 && t_date.m_day == 29)
    return true;

  if (t_date.m_day > days_of_month[t_date.m_month - 1]) return false;

  return true;
}

/**
 * @brief convert a string to date, if the format is not correct return
 * 0000-00-00/00:00
 * @return a date
 */
Date Date::stringToDate(const string &t_dateString) {
  std::regex pattern("^(\\d{4})\\-(\\d{2})\\-(\\d{2})/(\\d{2}):(\\d{2})$");
  std::smatch result;

  if (!std::regex_match(t_dateString, result, pattern))
    return Date(0, 0, 0, 0, 0);

  Date ret(stoi(result[1]), stoi(result[2]), stoi(result[3]), stoi(result[4]),
           stoi(result[5]));

  if (!isValid(ret)) {
    return Date(0, 0, 0, 0, 0);
  }

  return ret;
}

/**
 * @brief convert a date to string, if the date is invalid return
 * 0000-00-00/00:00
 */
string Date::dateToString(const Date &t_date) {
  if (!isValid(t_date)) {
    return "0000-00-00/00:00";
  }

  string result =
      to_string(t_date.m_year) + '-' + formatString(t_date.m_month) + '-' +
      formatString(t_date.m_day) + '/' + formatString(t_date.m_hour) + ':' +
      formatString(t_date.m_minute);

  return result;
}

/**
 *  @brief overload the assign operator
 */
Date &Date::operator=(const Date &t_date) {
  this->m_year = t_date.m_year;
  this->m_month = t_date.m_month;
  this->m_day = t_date.m_day;
  this->m_hour = t_date.m_hour;
  this->m_minute = t_date.m_minute;

  return *this;
}

/**
 * @brief check whether the CurrentDate is equal to the t_date
 */
bool Date::operator==(const Date &t_date) const {
  return Date::dateToString(*this) == Date::dateToString(t_date);
}

/**
 * @brief check whether the CurrentDate is  greater than the t_date
 */
bool Date::operator>(const Date &t_date) const {
  return Date::dateToString(*this) > Date::dateToString(t_date);
}

/**
 * @brief check whether the CurrentDate is  less than the t_date
 */
bool Date::operator<(const Date &t_date) const { return !(*this >= t_date); }

/**
 * @brief check whether the CurrentDate is  greater or equal than the t_date
 */
bool Date::operator>=(const Date &t_date) const {
  return (*this > t_date || *this == t_date);
}

/**
 * @brief check whether the CurrentDate is  less than or equal to the t_date
 */
bool Date::operator<=(const Date &t_date) const { return !(*this > t_date); }