#include "../include/eeprom24cxx.hpp"
#include "./clear.cpp"
#include "./include/base-command.hpp"
#include "./read.cpp"
#include "./write.cpp"
#include "command.hpp"

class MainCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description(
            "This command allows to clear, read and write an eeprom AT24CXX.")
        ->add("-t, --type <at24cxx>",
              "Selects one of the avaliable device types:\nAT24C01, AT24C02, "
              "AT24C04, "
              "AT24C08,\n AT24C16, AT24C32, AT24C64, AT24C128,\n "
              "AT24C256.\nDefaults to AT24C16.\n\nExample: \"-t 32\".")
        ->add("i2c-bus", "File to the I2C bus.\nEx: /dev/i2c-1.")
        ->add("dev-addr", "Device address.")
        ->add("read", new ReadCommand())
        ->add("clear", new ClearCommand())
        ->add("write", new WriteCommand());
  }
};

int main(int argc, char *argv[]) {
  auto command = MainCommand();
  return command.from_main(argc, argv);
  return 0;
}
