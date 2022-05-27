#include "../include/eeprom24cxx.hpp"
#include "./clear.cpp"
#include "./commander/include/command.hpp"
#include "./include/base-command.hpp"
#include "./read.cpp"

class MainCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description(
            "This command allows to clear, read and write an eeprom AT24CXX.")
        ->add("-t, --type <at24cxx>", "Selects one of the avaliable types.")
        ->add("i2c-bus", "File to the I2C bus.\nEx: /dev/i2c-1.")
        ->add("dev-addr", "Device address.")
        ->add("read", new ReadCommand())
        ->add("clear", new ClearCommand());
  }
};

int main(int argc, char *argv[]) {
  auto command = MainCommand();
  return command.from_main(argc, argv);
  return 0;
}
