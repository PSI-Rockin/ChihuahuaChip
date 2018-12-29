#ifndef EMULATOR_HPP
#define EMULATOR_HPP
#include "cpu.hpp"

class Emulator
{
    private:
        CPU cpu;

        uint8_t memory[4096];
        uint32_t framebuffer[64 * 32];
    public:
        Emulator();

        uint8_t* get_framebuffer();

        void reset();
        void run();
        void load_rom(uint8_t* ROM, uint32_t size);
};

inline uint8_t* Emulator::get_framebuffer()
{
    return (uint8_t*)framebuffer;
}

#endif // EMULATOR_HPP
