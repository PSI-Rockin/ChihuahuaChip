#include <cstring>
#include "emulator.hpp"

Emulator::Emulator() : cpu((uint8_t*)memory, (uint32_t*)framebuffer)
{

}

void Emulator::reset()
{
    memset(memory, 0xFF, 0x200);
    for (int i = 0; i < 64 * 32; i++)
        framebuffer[i] = 0xFF000000;
    cpu.reset();
}

void Emulator::run()
{
    cpu.run(10000000);
    cpu.dec_delay();
}

void Emulator::load_rom(uint8_t *ROM, uint32_t size)
{
    if (size > 4096 - 512)
        return;

    memcpy(memory + 0x200, ROM, size);
}
