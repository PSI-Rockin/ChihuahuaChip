#ifndef CPU_HPP
#define CPU_HPP
#include <cstdint>

class CPU
{
    private:
        uint8_t delay;
    public:
        int cycles;
        uint32_t PC;
        uint8_t gpr[16];
        uint8_t* memory;
        uint16_t call_stack[32];
        uint16_t I;
        uint8_t SP;
        uint32_t *framebuffer;

        CPU(uint8_t* memory, uint32_t* framebuffer);

        uint16_t read16(uint16_t addr);
        uint16_t get_PC();
        uint8_t get_delay();
        void set_PC(uint16_t pc);
        void set_delay(uint8_t value);

        void reset();
        void run(int cycles);
        void dec_delay();

        void clear();
        void ret();
};

#endif // CPU_HPP
