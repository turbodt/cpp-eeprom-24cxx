#ifndef H_INCLUDE_CLI_BASE_COMMAND
#define H_INCLUDE_CLI_BASE_COMMAND

#include "../include/command.hpp"
#include "../include/eeprom24cxx.hpp"
#include "./utils.hpp"

class BaseCommand : public Commander::Command {
public:
  using Commander::Command::Command;

protected:
  std::string get_i2c_bus() const {
    return this->get_global_positional(0)->get_value();
  }

  std::uint8_t get_device_address() const {
    std::string device_address_str =
        this->get_global_positional(1)->get_value();

    std::uint8_t device_address = utils::to_literal(device_address_str) & 0xFF;
    return device_address;
  }

  EepromAT24CXX::at24cxx_t get_device_type() const {
    EepromAT24CXX::at24cxx_t t = EepromAT24CXX::AT24C16;
    if (this->get_global_option("--type")->get_value(0)->has_value()) {
      auto type_int = utils::to_literal(
          this->get_global_option("--type")->get_value(0)->get_value());
      if (type_int == 1) {
        return EepromAT24CXX::AT24C01;
      } else if (type_int == 2) {
        return EepromAT24CXX::AT24C02;
      } else if (type_int == 4) {
        return EepromAT24CXX::AT24C04;
      } else if (type_int == 8) {
        return EepromAT24CXX::AT24C08;
      } else if (type_int == 16) {
        return EepromAT24CXX::AT24C16;
      } else if (type_int == 32) {
        return EepromAT24CXX::AT24C32;
      } else if (type_int == 64) {
        return EepromAT24CXX::AT24C64;
      } else if (type_int == 128) {
        return EepromAT24CXX::AT24C128;
      } else if (type_int == 256) {
        return EepromAT24CXX::AT24C256;
      }
    }
    return t;
  }

  BaseCommand *initialize_wrapper() {
    this->add("-h, --help", "Shows help.");
    this->Commander::Command::initialize_wrapper();
    return this;
  }

  int execute_wrapper(Commander::ArgumentList *list) const {
    if (this->get_option("--help")->has_appeared()) {
      std::cout << this->get_help() << std::endl;
      return 0;
    }
    return this->Command::execute_wrapper(list);
  }
};
#endif
