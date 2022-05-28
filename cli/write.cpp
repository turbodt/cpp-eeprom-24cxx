#include "../include/eeprom24cxx.hpp"
#include "./commander/include/command.hpp"
#include "./include/base-command.hpp"
#include "./include/utils.hpp"

class WriteCommand : public BaseCommand {
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

  std::uint16_t get_from() const {
    auto device_size =
        EepromAT24CXX::EepromAT24CXX::get_features(this->get_device_type())
            ->total_bytes;
    std::uint16_t from = 0x0000;
    if (this->get_positional("from")->has_value()) {
      from =
          std::min(utils::to_literal(this->get_positional("from")->get_value()),
                   (int)device_size);
    }
    return from;
  }

  std::vector<std::uint8_t> get_data() const {
    auto device_size =
        EepromAT24CXX::EepromAT24CXX::get_features(this->get_device_type())
            ->total_bytes;
    auto buffer = new std::uint8_t[device_size]();
    auto data = std::vector<std::uint8_t>();

    std::freopen(nullptr, "rb", stdin);
    std::size_t len;
    while (data.size() < device_size &&
           ((len = std::fread(buffer, sizeof(std::uint8_t),
                              device_size - data.size(), stdin)) > 0)) {

      if (std::ferror(stdin) && !std::feof(stdin)) {
        std::cerr << "Error happens." << std::endl;
      } else {
        data.insert(data.end(), buffer, buffer + len);
      }
    }

    delete[] buffer;
    return data;
  }

  int execute() const {
    auto eeprom = new EepromAT24CXX::EepromAT24CXX(this->get_device_type());
    eeprom->bind(this->get_i2c_bus().c_str(), this->get_device_address());
    auto from = this->get_from();
    auto data = this->get_data();
    eeprom->write(from, &data);
    delete eeprom;

    return 0;
  };
};
