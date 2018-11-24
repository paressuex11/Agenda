#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>

class my_exception {
 public:
  my_exception(const std::string &msg) : m_message(msg) {}

  virtual ~my_exception() {}

  std::string what() const { return this->m_message; }

  virtual std::string type() const = 0;

 private:
  std::string m_message;
};

class wrong_format : public my_exception {
 public:
  wrong_format(const std::string &msg = "") : my_exception(msg){};

  virtual std::string type() const { return "Wrong Error"; }
};

class invalid_date : public my_exception {
 public:
  invalid_date(const std::string &msg = "") : my_exception(msg){};

  virtual std::string type() const { return "Invalid Date"; }
};

class user_not_found : public my_exception {
 public:
  user_not_found(const std::string &msg = "") : my_exception(msg) {}

  virtual std::string type() const { return "User Not Found"; }
};

class user_repeat : public my_exception {
 public:
  user_repeat(const std::string &msg = "") : my_exception(msg) {}

  virtual std::string type() const { return "User Repeat"; }
};

class title_repeat : public my_exception {
 public:
  title_repeat(const std::string &msg = std::string("")) : my_exception(msg) {}

  virtual std::string type() const { return "Title Repeat"; }
};

class time_conflict : public my_exception {
 public:
  time_conflict(const std::string &msg = std::string("")) : my_exception(msg) {}

  virtual std::string type() const { return "Time Confliction"; }
};

class meeting_not_found : public my_exception {
 public:
  meeting_not_found(const std::string &msg = "") : my_exception(msg) {}

  virtual std::string type() const { return "Meeting Not Found"; }
};

class empty_deletion : public my_exception {
 public:
  empty_deletion(const std::string &msg = "") : my_exception(msg) {}

  virtual std::string type() const { return "Empty Deletion"; }
};

class permission_error : public my_exception {
 public:
  permission_error(const std::string &msg = "") : my_exception(msg) {}

  virtual std::string type() const { return "Permission Error"; }
};

class wrong_arg_num : public my_exception {
 public:
  wrong_arg_num(const std::string &msg = "") : my_exception(msg) {}

  virtual std::string type() const { return "Wrong Argument Number"; }
};

class unknown_command : public my_exception {
 public:
  unknown_command(const std::string &msg = std::string(""))
      : my_exception(msg) {}

  virtual std::string type() const { return "Unknown Command"; }
};

#endif