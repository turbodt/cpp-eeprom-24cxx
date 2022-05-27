#include "../include/eeprom24cxx.hpp"
#include "./commander/include/command.hpp"
#include "./include/base-command.hpp"
#include "./include/utils.hpp"

class ReadCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description("This command reads the Eeprom Memory and outputs "
                          "its content to the standard output in binary mode.")
        ->add("[from]", "Address where to start. Defaults to 0x0000.")
        ->add("[length]",
              "Number of bits to show. Defaults to "
              "the remaining quantity to the end of the device memory.");
  }

  int get_from(EepromAT24CXX::EepromAT24CXX const *eeprom) const {
    std::uint16_t from = 0x0000;
    if (this->get_positional("from")->has_value()) {
      from =
          std::min(utils::to_literal(this->get_positional("from")->get_value()),
                   (int)eeprom->size());
    }
    return from;
  }

  int get_length(EepromAT24CXX::EepromAT24CXX const *eeprom) const {
    std::uint16_t length = eeprom->size() - this->get_from(eeprom);
    if (this->get_positional("length")->has_value()) {
      length = std::min(
          utils::to_literal(this->get_positional("length")->get_value()),
          (int)length);
    }
    return length;
  }

  int execute() const {
    auto eeprom = new EepromAT24CXX::EepromAT24CXX(this->get_device_type());
    eeprom->bind(this->get_i2c_bus().c_str(), this->get_device_address());
    auto from = this->get_from(eeprom);
    auto length = this->get_length(eeprom);
    std::cout << eeprom->read_str(from, length) << std::flush;
    delete eeprom;
    return 0;
  };
};
