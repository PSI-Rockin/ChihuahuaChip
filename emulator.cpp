#include <cstring>
#include "emulator.hpp"

Emulator::Emulator() : cpu((uint8_t*)memory)
{

}

void Emulator::reset()
{
    cpu.reset();
}

void Emulator::run()
{
    for (int i = 0; i < 500; i++)
        cpu.run();
}

void Emulator::load_rom(uint8_t *ROM, uint32_t size)
{
    if (size > 4096 - 512)
        return;

    memcpy(memory + 0x200, ROM, size);
}
