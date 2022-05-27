#ifndef H_INCLUDE_EEPROM_AT24CXX
#define H_INCLUDE_EEPROM_AT24CXX

#include <fcntl.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}

#include "exceptions.hpp"

namespace EepromAT24CXX {

typedef std::uint16_t at24cxx_t;

const at24cxx_t AT24C01 = 0x0001;
const at24cxx_t AT24C02 = 0x0002;
const at24cxx_t AT24C04 = 0x0004;
const at24cxx_t AT24C08 = 0x0008;
const at24cxx_t AT24C16 = 0x0010;
const at24cxx_t AT24C32 = 0x0020;
const at24cxx_t AT24C64 = 0x0040;
const at24cxx_t AT24C128 = 0x0080;
const at24cxx_t AT24C256 = 0x0100;

struct Features {
  std::uint8_t file_addr_bytes;
  std::uint8_t bytes_per_page;
  std::uint16_t total_bytes;
  std::uint8_t write_cycle_time_ms;
};

const std::map<at24cxx_t const, Features const> FEATURES = {
    {AT24C01, {1, 16, 256, 5}},     {AT24C02, {1, 16, 256, 5}},
    {AT24C04, {1, 16, 256, 5}},     {AT24C08, {1, 16, 256, 5}},
    {AT24C16, {1, 16, 256, 5}},     {AT24C32, {2, 32, 4096, 10}},
    {AT24C64, {2, 32, 8192, 10}},   {AT24C128, {2, 64, 16384, 10}},
    {AT24C256, {2, 64, 32768, 10}},
};

class EepromAT24CXX {
private:
  at24cxx_t type;
  int file_descriptor;
  void assert_binded_device() const;

public:
  EepromAT24CXX(at24cxx_t);
  ~EepromAT24CXX();
  // getters
  at24cxx_t const &get_type() const;
  Features const *get_features() const;
  std::uint16_t size() const;
  // Connect with device
  EepromAT24CXX *bind(char const *, std::uint8_t address);
  EepromAT24CXX *unbind();
  // Device internal state
  EepromAT24CXX const *go_to(std::uint16_t const) const;
  // Read
  std::uint8_t read_next_byte() const;
  std::uint8_t read_byte(std::uint16_t const) const;
  std::vector<std::uint8_t> read_next(std::uint16_t const) const;
  std::vector<std::uint8_t> read(std::uint16_t const,
                                 std::uint16_t const) const;
  std::vector<std::uint8_t> read_all() const;

  std::string read_str(std::uint16_t const, std::uint16_t const) const;
  std::string read_all_str() const;

  // Write
  EepromAT24CXX const *write_byte(std::uint16_t const, uint8_t const) const;

  EepromAT24CXX const *write(std::uint16_t const,
                             std::vector<uint8_t> const *) const;
  EepromAT24CXX const *write(std::uint16_t const, std::uint16_t const,
                             std::uint8_t const *) const;
  EepromAT24CXX const *write(std::uint16_t const, std::string const &) const;
  EepromAT24CXX const *clear() const;
  EepromAT24CXX const *clear(std::uint8_t const) const;
};
} // namespace EepromAT24CXX

#endif
