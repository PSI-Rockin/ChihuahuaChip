#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
#include "cpu.hpp"

namespace Interpreter
{
    void interpret(CPU* cpu, uint16_t instr);

    void clear(CPU* cpu);
    void ret(CPU* cpu);
    void jmp(CPU* cpu, uint16_t instr);
    void call(CPU* cpu, uint16_t instr);
    void skpe(CPU* cpu, uint16_t instr);
    void skpn(CPU* cpu, uint16_t instr);
    void ld_imm(CPU* cpu, uint16_t instr);
    void add_imm(CPU* cpu, uint16_t instr);
    void ld_reg(CPU* cpu, uint16_t instr);
    void and_reg(CPU* cpu, uint16_t instr);
    void xor_reg(CPU* cpu, uint16_t instr);
    void adc_reg(CPU* cpu, uint16_t instr);
    void sbc_reg(CPU* cpu, uint16_t instr);
    void srl(CPU* cpu, uint16_t instr);
    void sll(CPU* cpu, uint16_t instr);
    void ld_i(CPU* cpu, uint16_t instr);
    void rand_cpu(CPU* cpu, uint16_t instr);
    void draw_cpu(CPU* cpu, uint16_t instr);

    void skpn_key(CPU* cpu, uint16_t instr);
    void ld_reg_delay(CPU* cpu, uint16_t instr);
    void ld_delay_reg(CPU* cpu, uint16_t instr);
    void add_i(CPU* cpu, uint16_t instr);
    void sprite(CPU* cpu, uint16_t instr);
    void bcd(CPU* cpu, uint16_t instr);
    void reg_to_mem(CPU* cpu, uint16_t instr);
    void mem_to_reg(CPU* cpu, uint16_t instr);
};

#endif // INTERPRETER_HPP
