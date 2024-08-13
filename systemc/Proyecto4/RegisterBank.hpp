#ifndef REGISTER_BANK_HPP
#define REGISTER_BANK_HPP

#include <vector>
#include <stdexcept>
#include <cstdint>

#define REGBANK_SIZE 0xAC

class RegisterBank {
public:
    // Constructor to initialize the register bank
    RegisterBank(uint32_t base_addr, uint32_t end_addr);

    // Read the value of a specific register
    uint32_t read(uint32_t addr, uint32_t size);

    void write(uint32_t addr, void* value, uint32_t size);

private:
    uint8_t memory[REGBANK_SIZE];
    uint32_t base_address;
    uint32_t end_address;

    // Helper function to get register index from address
    uint32_t get_register_index(uint32_t addr);
};

#endif // REGISTER_BANK_HPP
