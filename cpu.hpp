#ifndef CPU_HPP
#define CPU_HPP
#include <cstdint>

class CPU
{
    private:
        uint8_t gpr[16];
        uint16_t PC;
        uint16_t I;

        uint8_t* memory;
    public:
        CPU(uint8_t* memory);

        void reset();
        void run();
};

#endif // CPU_HPP
