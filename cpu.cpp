#include <cstdio>
#include <cstdlib>
#include "cpu.hpp"

CPU::CPU(uint8_t* memory) : memory(memory)
{

}

void CPU::reset()
{
    PC = 0x200;
}

void CPU::run()
{
    uint16_t instr = memory[PC + 1];
    instr |= ((uint16_t)memory[PC]) << 8;

    PC += 2;

    switch (instr)
    {
        default:
            printf("[CPU] Unrecognized instruction $%04X\n", instr);
            exit(1);
    }
}
