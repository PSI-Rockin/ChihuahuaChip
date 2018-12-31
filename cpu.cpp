#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "cpu.hpp"
#include "interpreter.hpp"

#define printf(fmt, ...)(0)

CPU::CPU(uint8_t* memory, uint32_t* framebuffer) : memory(memory), framebuffer(framebuffer)
{

}

uint16_t CPU::read16(uint16_t addr)
{
    uint16_t value = memory[addr + 1];
    value |= ((uint16_t)memory[addr]) << 8;
    return value;
}

uint16_t CPU::get_PC()
{
    return PC;
}

uint8_t CPU::get_delay()
{
    return delay;
}

void CPU::set_PC(uint16_t pc)
{
    PC = pc;
}

void CPU::set_delay(uint8_t value)
{
    delay = value;
}

void CPU::reset()
{
    srand(time(NULL));
    PC = 0x200;
    delay = 0;
    cycles = 0;
    I = 0;
    SP = 0;
}

void CPU::run(int max_cycles)
{
    while (cycles < max_cycles)
    {
        cycles++;
        uint16_t instr = memory[PC + 1];
        instr |= ((uint16_t)memory[PC]) << 8;

        printf("[$%04X] $%04X ", PC, instr);

        PC += 2;

        Interpreter::interpret(this, instr);
    }
}

void CPU::dec_delay()
{
    if (delay)
        delay--;
}

void CPU::clear()
{
    for (int i = 0; i < 64 * 32; i++)
        framebuffer[i] = 0xFF000000;
}

void CPU::ret()
{
    SP--;
    PC = call_stack[SP];
}
