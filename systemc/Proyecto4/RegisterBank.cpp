#include <string.h>

#include "RegisterBank.hpp"

// Constructor to initialize the register bank with a specific number of registers
RegisterBank::RegisterBank(uint32_t base_addr, uint32_t end_addr)
    : base_address(base_addr), end_address(end_addr) 
{
    
}

uint32_t RegisterBank::read(uint32_t addr, uint32_t size)
{
    uint32_t data = 0;
    addr = get_register_index(addr);
    memcpy(&data, &memory[addr], size);
    return data;
}

void RegisterBank::write(uint32_t addr, void* value, uint32_t size) 
{
    addr = get_register_index(addr);
    memcpy(&memory[addr], value, size);
}

//Private function. To help other methods to find the correct index of a register
//This helps to have a clearer view of the Memory Bank created during Project 1
uint32_t RegisterBank::get_register_index(uint32_t addr) 
{
    if (addr < base_address || addr > end_address) {
        throw std::out_of_range("Address out of range");
    }
    return (addr - base_address);
}
