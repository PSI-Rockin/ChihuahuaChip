#ifndef JITTRANSLATE_HPP
#define JITTRANSLATE_HPP
#include "cpu.hpp"
#include "emitterx64.hpp"

namespace JitTranslate
{
    void set_cpu(CPU* c);
    void run();
    void reset();

    void flush_gprs();
    uint8_t* exec_block();
    void recompile_block();

    void jump(uint16_t addr);
};

#endif // JITTRANSLATE_HPP
