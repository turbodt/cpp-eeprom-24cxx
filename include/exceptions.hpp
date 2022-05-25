#ifndef H_INCLUDE_EEPROM_AT24CXX_EXCEPTIONS
#define H_INCLUDE_EEPROM_AT24CXX_EXCEPTIONS

#include <exception>
#include <iostream>

namespace EepromAT24CXX {

class UnknownException : public std::exception {
protected:
  std::string message;

public:
  explicit UnknownException(){};
  explicit UnknownException(const std::string &message) : message(message){};
  virtual const char *what() const noexcept { return this->message.c_str(); }
  void update_message(const std::string &message) { this->message = message; }
};

class UnbindDeviceException : public UnknownException {
public:
  explicit UnbindDeviceException()
      : UnknownException(
            "EepromAT24CXX interface is not binded to any device."){};
  explicit UnbindDeviceException(const std::string &message)
      : UnknownException(message){};
};

class UnexpectedPropertyValueException : public UnknownException {
public:
  explicit UnexpectedPropertyValueException(const std::string &message)
      : UnknownException(message){};
};

class OpenException : public UnknownException {
public:
  explicit OpenException(const std::string &message)
      : UnknownException(message){};
};
} // namespace EepromAT24CXX

#endif
