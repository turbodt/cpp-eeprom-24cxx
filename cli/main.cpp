#include "../include/eeprom24cxx.hpp"
#include "./clear.cpp"
#include "./commander/include/command.hpp"
#include "./read.cpp"

class MainCommand : public Commander::Command {
public:
  using Commander::Command::Command;

  void initialize() {
    this->set_description(
            "This command allows to clear, read and write an eeprom AT24CXX.")
        ->add_positional("i2c-bus", "File to the I2C bus. Ex: /dev/i2c-1.")
        ->add_positional("dev-addr", "Device address.")
        ->add_subcommand("read", new ReadCommand())
        ->add_subcommand("clear", new ClearCommand());
  }
};

int main(int argc, char *argv[]) {
  auto command = MainCommand();
  return command.from_main(argc, argv);
}
