#ifndef EMULATOR_HPP
#define EMULATOR_HPP
#include "cpu.hpp"

class Emulator
{
    private:
        CPU cpu;

        uint8_t memory[4096];
    public:
        Emulator();

        void reset();
        void run();
        void load_rom(uint8_t* ROM, uint32_t size);
};

#endif // EMULATOR_HPP
