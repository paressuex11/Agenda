#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>

class my_exception {
 public:
  my_exception(const std::string &p_msg) : m_message(p_msg) {}

  virtual ~my_exception() {}

  std::string what() const { return this->m_message; }

  virtual std::string type() const = 0;

 private:
  std::string m_message;
};

class permission_error : public my_exception {
 public:
  permission_error(const std::string &p_msg = "") : my_exception(p_msg) {}

  virtual std::string type() const { return "Permission Error"; }
};

class wrong_arg_num : public my_exception {
 public:
  wrong_arg_num(const std::string &p_msg = "") : my_exception(p_msg) {}

  virtual std::string type() const { return "Wrong Argument Number"; }
};

class unknown_command : public my_exception {
 public:
  unknown_command(const std::string &p_msg = std::string(""))
      : my_exception(p_msg) {}

  virtual std::string type() const { return "Unknown Command"; }
};

#endif