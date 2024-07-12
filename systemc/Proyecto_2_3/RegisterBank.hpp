#ifndef REGISTER_BANK_HPP
#define REGISTER_BANK_HPP

#include <vector>
#include <stdexcept>
#include <cstdint>

class RegisterBank {
public:
    // Constructor to initialize the register bank
    RegisterBank(uint32_t base_addr, uint32_t end_addr);

    // Read the value of a specific register
    uint32_t read_register(uint32_t addr) const;

    // Write a value to a specific register
    void write_register(uint32_t addr, uint32_t value);

    // Read a specific bit from a register
    bool read_bit(uint32_t addr, uint8_t bit) const;

    // Write a specific bit to a register
    void write_bit(uint32_t addr, uint8_t bit, bool value);

    // Read multiple bits using a mask
    uint32_t read_bits(uint32_t addr, uint32_t mask) const;

    // Write multiple bits using a mask
    void write_bits(uint32_t addr, uint32_t mask, uint32_t value);

    // Reset all registers to a default value
    void reset_registers(uint32_t default_value = 0);

private:
    std::vector<uint32_t> registers;
    uint32_t base_address;
    uint32_t end_address;

    // Helper function to get register index from address
    size_t get_register_index(uint32_t addr) const;
};

#endif // REGISTER_BANK_HPP
