#include "../include/eeprom24cxx.hpp"
#include "./commander/include/command.hpp"
#include "./include/base-command.hpp"

class ReadCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description("This command reads the Eeprom Memory and outputs "
                          "its content to the standard output in binary mode.")
        ->add_positional("[from]",
                         "Address where to start. Defaults to 0x0000.")
        ->add_positional("[to]",
                         "Address where to end, not included. Defaults to "
                         "one more than last file address of the device.");
  }

  int execute() const {
    auto eeprom = new EepromAT24CXX::EepromAT24CXX(EepromAT24CXX::AT24C16);
    eeprom->bind(this->get_i2c_bus().c_str(), this->get_device_address());
    std::uint16_t from = 0x0000;
    std::uint16_t to = eeprom->size();

    std::cout << eeprom->read_all_str() << std::flush;
    delete eeprom;
    return 0;
  };
};
