#include "../include/eeprom24cxx.hpp"
#include "./include/base-command.hpp"

class ClearCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description("This command erase the Eeprom Memory.")
        ->add("[value]",
              "Clear memory by setting all file addresses to that value.");
  }

  std::uint8_t get_value() const {
    std::uint8_t value = 0xff;
    if (this->get_positional("value")->has_value()) {
      value =
          utils::to_literal(this->get_positional("value")->get_value()) % 0x100;
    }
    return value;
  }

  int execute() const {
    auto eeprom = new EepromAT24CXX::EepromAT24CXX(EepromAT24CXX::AT24C16);
    eeprom->bind(this->get_i2c_bus().c_str(), this->get_device_address());
    eeprom->clear(this->get_value());
    return 0;
  };
};
