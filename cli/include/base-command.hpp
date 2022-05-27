#ifndef H_INCLUDE_CLI_BASE_COMMAND
#define H_INCLUDE_CLI_BASE_COMMAND

#include "../include/command.hpp"
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
