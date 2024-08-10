#include "RegisterBank.hpp"

// Constructor to initialize the register bank with a specific number of registers
RegisterBank::RegisterBank(uint32_t base_addr, uint32_t end_addr)
    : base_address(base_addr), end_address(end_addr) 
{
    size_t num_registers = (end_addr - base_addr) / 4 + 1;
    registers.resize(num_registers, 0);
}

uint32_t RegisterBank::read_register(uint32_t addr) const 
{
    size_t index = get_register_index(addr);
    return registers.at(index);
}

void RegisterBank::write_register(uint32_t addr, uint32_t value) 
{
    size_t index = get_register_index(addr);
    registers.at(index) = value;
}

bool RegisterBank::read_bit(uint32_t addr, uint8_t bit) const 
{
    if (bit >= 32) {
        throw std::out_of_range("Bit index out of range");
    }
    size_t index = get_register_index(addr);
    return (registers.at(index) & (1 << bit)) != 0;
}

void RegisterBank::write_bit(uint32_t addr, uint8_t bit, bool value) 
{
    if (bit >= 32) {
        throw std::out_of_range("Bit index out of range");
    }
    size_t index = get_register_index(addr);
    if (value) {
        registers.at(index) |= (1 << bit);  // Set the bit
    } else {
        registers.at(index) &= ~(1 << bit); // Clear the bit
    }
}

void RegisterBank::write_bits(uint32_t addr, uint32_t mask, uint32_t value) 
{
    size_t index = get_register_index(addr);
    // Clear the bits specified by the mask and set them to the new value
    registers.at(index) = (registers.at(index) & ~mask) | (value & mask);
}

uint32_t RegisterBank::read_bits(uint32_t addr, uint32_t mask) const 
{
    size_t index = get_register_index(addr);
    // Return only the bits specified by the mask
    return registers.at(index) & mask;
}

void RegisterBank::reset_registers(uint32_t default_value) 
{
    std::fill(registers.begin(), registers.end(), default_value);
}

//Private function. To help other methods to find the correct index of a register
//This helps to have a clearer view of the Memory Bank created during Project 1
size_t RegisterBank::get_register_index(uint32_t addr) const 
{
    if (addr < base_address || addr > end_address) {
        throw std::out_of_range("Address out of range");
    }
    return (addr - base_address) / 4;
}
