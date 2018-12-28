#ifndef CPU_HPP
#define CPU_HPP
#include <cstdint>

class CPU
{
    private:
        uint8_t gpr[16];
        uint16_t PC;
        uint16_t I;
        uint16_t call_stack[32];
        uint8_t SP;

        uint8_t delay;

        uint8_t* memory;
    public:
        CPU(uint8_t* memory);

        void reset();
        void run();
};

#endif // CPU_HPP
