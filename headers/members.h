#pragma once 
#include<cstdint>

class Chip8
{
  public:
    uint8_t registers[16]{}; // 16 one byte register  or 16 8-bit registers
                             
    uint8_t memory[4096]{}; //4096 bytes for memory (each byte is one space of memory from 0 to 4096, memory is stored in bits)

    uint16_t index{}; //pointer to where data is in memory 
    uint16_t pc{}; 
    uint16_t stack[16]{};
    uint8_t  sp{};
    uint8_t  delaytimer{};
    uint8_t  soundTimer{};
    uint8_t  keypad[16]{};
    uint32_t video[64*32]{};
    uint16_t opcode; //instruction 
};
