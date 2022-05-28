#include "../include/eeprom24cxx.hpp"

namespace EepromAT24CXX {

EepromAT24CXX::EepromAT24CXX(at24cxx_t type) : type(type), file_descriptor(0){};

EepromAT24CXX::~EepromAT24CXX() { this->unbind(); }

at24cxx_t const &EepromAT24CXX::get_type() const { return this->type; }
Features const *EepromAT24CXX::get_features() const {
  return EepromAT24CXX::get_features(this->get_type());
}

Features const *EepromAT24CXX::get_features(at24cxx_t type) {
  return &(FEATURES.at(type));
}

std::uint16_t EepromAT24CXX::size() const {
  return this->get_features()->total_bytes;
}

void EepromAT24CXX::assert_binded_device() const {
  if (this->file_descriptor <= 0) {
    throw UnbindDeviceException();
  }
};

EepromAT24CXX *EepromAT24CXX::bind(char const *file, std::uint8_t address) {
  this->unbind();
  this->file_descriptor = open(file, O_RDWR);

  if (this->file_descriptor < 0) {
    this->unbind();
    throw OpenException("Couldn't open I2C bus `" + std::string(file) + "`.");
  }

  if (ioctl(this->file_descriptor, I2C_SLAVE, address) < 0) {
    this->unbind();
    throw OpenException("On I2C bus `" + std::string(file) +
                        "` couldn't find address " + std::to_string(address) +
                        ".");
  }

  return this;
}

EepromAT24CXX *EepromAT24CXX::unbind() {
  if (this->file_descriptor > 0) {
    close(this->file_descriptor);
  }
  this->file_descriptor = 0;
  return this;
}

EepromAT24CXX const *
EepromAT24CXX::go_to(std::uint16_t const file_address) const {
  this->assert_binded_device();
  if (this->get_features()->file_addr_bytes == 1) {
    // effectivelly, writes one byte
    i2c_smbus_write_byte(this->file_descriptor,
                         (std::uint8_t)(0x00FF & file_address));
  } else if (this->get_features()->file_addr_bytes == 2) {
    // effectivelly, writes two bytes
    i2c_smbus_write_byte_data(this->file_descriptor,
                              (uint8_t)((0xFF00 & file_address) >> 8),
                              (uint8_t)(0x00FF & file_address));
  } else {
    throw UnexpectedPropertyValueException(
        "EepromAT24CXX of type " + std::to_string(this->get_type()) +
        " does not have expected `file_address` property.");
  }
  return this;
}

std::uint8_t EepromAT24CXX::read_next_byte() const {
  this->assert_binded_device();
  return i2c_smbus_read_byte(this->file_descriptor);
};

std::uint8_t EepromAT24CXX::read_byte(std::uint16_t const file_address) const {
  this->assert_binded_device();
  this->go_to(file_address);
  return i2c_smbus_read_byte(this->file_descriptor);
};

std::vector<std::uint8_t>
EepromAT24CXX::read_next(std::uint16_t const length) const {
  this->assert_binded_device();
  auto data = std::vector<std::uint8_t>(length);

  // TODO: Check if we can do it better than read byte by byte
  for (uint16_t i = 0; i < length; i++) {
    data.at(i) = this->read_next_byte();
  }
  return data;
};

std::vector<std::uint8_t>
EepromAT24CXX::read(std::uint16_t const file_address,
                    std::uint16_t const length) const {
  this->assert_binded_device();
  this->go_to(file_address);
  return this->read_next(length);
};

std::vector<std::uint8_t> EepromAT24CXX::read_all() const {
  this->assert_binded_device();
  return this->read(0x0000, this->get_features()->total_bytes);
}

std::string EepromAT24CXX::read_str(std::uint16_t const file_address,
                                    std::uint16_t const length) const {
  auto data = this->read(file_address, length);
  return std::string(data.begin(), data.end());
};

std::string EepromAT24CXX::read_all_str() const {
  auto data = this->read_all();
  return std::string(data.begin(), data.end());
}

EepromAT24CXX const *EepromAT24CXX::write_byte(std::uint16_t const file_address,
                                               uint8_t const value) const {
  this->assert_binded_device();

  if (this->get_features()->file_addr_bytes == 1) {
    // effectivelly, writes two bytes
    i2c_smbus_write_byte_data(this->file_descriptor, 0x00FF & file_address,
                              value);
  } else if (this->get_features()->file_addr_bytes == 2) {
    // effectivelly, writes three bytes
    i2c_smbus_write_word_data(
        this->file_descriptor, (uint8_t)((0xFF00 & file_address) >> 8),
        (uint16_t)((0x00FF & file_address) << 8) + (uint16_t)value);
  } else {
    throw UnexpectedPropertyValueException(
        "EepromAT24CXX of type " + std::to_string(this->get_type()) +
        " does not have expected `file_address` property.");
  }
  usleep(this->get_features()->write_cycle_time_ms * 1000);
  return this;
}

EepromAT24CXX const *EepromAT24CXX::write(std::uint16_t const file_address,
                                          std::uint16_t const length,
                                          std::uint8_t const *data) const {
  this->assert_binded_device();

  // break into blocks, if any
  std::uint8_t offset = file_address % this->get_features()->bytes_per_page;
  std::uint8_t block_length = this->get_features()->bytes_per_page - offset;
  if (block_length < length) {
    std::uint16_t remaining_length = length;
    std::uint16_t block_file_address = file_address;
    std::uint8_t const *block_data = data;
    while (remaining_length > 0) {
      // write block
      this->write(block_file_address, block_length, block_data);
      // update next block values
      remaining_length -= block_length;
      block_file_address += block_length;
      block_data += block_length;
      block_length = this->get_features()->bytes_per_page;
      if (block_length > remaining_length) {
        block_length = remaining_length;
      }
    }
    return this;
  }

  // copy address + data
  std::uint8_t new_data_length = length + this->get_features()->file_addr_bytes;
  std::uint8_t *new_data = new std::uint8_t[new_data_length];

  if (this->get_features()->file_addr_bytes == 1) {
    *(new_data) = (uint8_t)(file_address);
  } else if (this->get_features()->file_addr_bytes == 2) {
    *(new_data) = (uint8_t)((0xFF00 & file_address) >> 8);
    *(new_data + 1) = (uint8_t)(0x00FF & file_address);
  } else {
    throw UnexpectedPropertyValueException(
        "EepromAT24CXX of type " + std::to_string(this->get_type()) +
        " does not have expected `file_address` property.");
  }

  // copy data
  for (std::uint8_t i = 0; i < length; i++) {
    *(new_data + this->get_features()->file_addr_bytes + i) = *(data + i);
  };

  // perform write
  i2c_smbus_write_i2c_block_data(this->file_descriptor, *(new_data),
                                 new_data_length - 1, (new_data + 1));
  delete[] new_data;

  // wait write cycle
  usleep(this->get_features()->write_cycle_time_ms * 1000);
  return this;
}

EepromAT24CXX const *
EepromAT24CXX::write(std::uint16_t const file_address,
                     std::vector<uint8_t> const *values) const {
  return this->write(file_address, values->size(), values->data());
}

EepromAT24CXX const *EepromAT24CXX::write(std::uint16_t const file_address,
                                          std::string const &message) const {
  return this->write(file_address, message.size(),
                     (std::uint8_t *)message.data());
}

EepromAT24CXX const *EepromAT24CXX::clear(std::uint8_t const value) const {
  auto data = new std::vector(this->get_features()->total_bytes, value);
  this->write(0x0000, data);
  delete data;
  return this;
}
EepromAT24CXX const *EepromAT24CXX::clear() const { return this->clear(0xFF); }
} // namespace EepromAT24CXX
