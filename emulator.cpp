#include <cstring>
#include "emulator.hpp"
#include "jittranslate.hpp"

Emulator::Emulator() : cpu((uint8_t*)memory, (uint32_t*)framebuffer)
{
    JitTranslate::set_cpu(&cpu);
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
    int max_cycles = 100000;
    /*while (cpu.cycles < max_cycles)
    {
        JitTranslate::run();
    }*/
    cpu.run(max_cycles);
    cpu.dec_delay();
    cpu.cycles -= max_cycles;
}

void Emulator::load_rom(uint8_t *ROM, uint32_t size)
{
    if (size > 4096 - 512)
        return;

    memcpy(memory + 0x200, ROM, size);
}
