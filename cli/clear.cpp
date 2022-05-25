#include "../include/eeprom24cxx.hpp"
#include "./include/base-command.hpp"

class ClearCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description("This command erase the Eeprom Memory.");
  }

  int execute() const {
    std::cout << "Clearing " << this->get_i2c_bus() << " at "
              << (int)this->get_device_address() << std::endl;
    return 0;
  };
};
