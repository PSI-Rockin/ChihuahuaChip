#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "interpreter.hpp"

#define printf(fmt, ...)(0)

namespace Interpreter
{

void interpret(CPU* cpu, uint16_t instr)
{
    //printf("Interpret $%04X\n", instr);
    switch (instr & 0xF000)
    {
        case 0x0000:
            switch (instr & 0xFFF)
            {
                case 0x0E0:
                    printf("CLEAR");
                    clear(cpu);
                    break;
                case 0x0EE:
                    printf("RET");
                    ret(cpu);
                    break;
                default:
                    printf("Unrecognized 0000 instruction $%04X\n", instr & 0xFFF);
                    exit(1);
            }
            break;
        case 0x1000:
            printf("JMP $%04X", instr & 0xFFF);
            jmp(cpu, instr);
            break;
        case 0x2000:
            printf("CALL $%04X", instr & 0xFFF);
            call(cpu, instr);
            break;
        case 0x3000:
            printf("SKPE V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
            skpe(cpu, instr);
            break;
        case 0x4000:
            printf("SKPN V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
            skpn(cpu, instr);
            break;
        case 0x6000:
            printf("LD V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
            ld_imm(cpu, instr);
            break;
        case 0x7000:
            printf("ADD V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
            add_imm(cpu, instr);
            break;
        case 0x8000:
            switch (instr & 0xF)
            {
                case 0x0:
                    printf("LD V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    ld_reg(cpu, instr);
                    break;
                case 0x2:
                    printf("AND V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    and_reg(cpu, instr);
                    break;
                case 0x3:
                    printf("XOR V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    xor_reg(cpu, instr);
                    break;
                case 0x4:
                    printf("ADC V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    adc_reg(cpu, instr);
                    break;
                case 0x5:
                    printf("SBC V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    sbc_reg(cpu, instr);
                    break;
                case 0x6:
                    printf("SRL V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    srl(cpu, instr);
                    break;
                case 0xE:
                    printf("SLL V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    sll(cpu, instr);
                    break;
                default:
                    printf("Unrecognized 8000 instruction $%04X\n", instr & 0xF);
                    exit(1);
            }
            break;
        case 0xA000:
            printf("LD I, $%04X", instr & 0xFFF);
            ld_i(cpu, instr);
            break;
        case 0xC000:
            printf("RAND V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
            rand_cpu(cpu, instr);
            break;
        case 0xD000:
            printf("DRAW V%01X, V%01X, $%02X", (instr >> 8) & 0xF, (instr >> 4) & 0xF, instr & 0xF);
            draw_cpu(cpu, instr);
            break;
        case 0xE000:
            switch (instr & 0xFF)
            {
                case 0xA1:
                    printf("SKPN V%01X, KEYPAD", (instr >> 8) & 0xF);
                    skpn_key(cpu, instr);
                    break;
                default:
                    printf("Unrecognized E000 instruction $%04X\n", instr & 0xFF);
                    exit(1);
            }
            break;
        case 0xF000:
            switch (instr & 0xFF)
            {
                case 0x07:
                    printf("LD V%01X, DELAY", (instr >> 8) & 0xF);
                    ld_reg_delay(cpu, instr);
                    break;
                case 0x15:
                    printf("LD DELAY, V%01X", (instr >> 8) & 0xF);
                    ld_delay_reg(cpu, instr);
                    break;
                case 0x18:
                    printf("LD SOUND, V%01X", (instr >> 8) & 0xF);
                    break;
                case 0x1E:
                    printf("ADD I, V%01X", (instr >> 8) & 0xF);
                    add_i(cpu, instr);
                    break;
                case 0x29:
                    printf("SPRITE I, V%01X", (instr >> 8) & 0xF);
                    sprite(cpu, instr);
                    break;
                case 0x33:
                    printf("BCD V%01X\n", (instr >> 8) & 0xF);
                    bcd(cpu, instr);
                    break;
                case 0x55:
                    printf("MEMCPY(MEM[I], cpu->gpr, %d)", ((instr >> 8) & 0xF) + 1);
                    reg_to_mem(cpu, instr);
                    break;
                case 0x65:
                    printf("MEMCPY(cpu->gpr, MEM[I], %d)", ((instr >> 8) & 0xF) + 1);
                    mem_to_reg(cpu, instr);
                    break;
                default:
                    printf("Unrecognized F000 instruction $%04X\n", instr & 0xFF);
                    exit(1);
            }
            break;
        default:
            printf("Unrecognized instruction $%04X\n", instr & 0xF000);
            exit(1);
    }

    printf("\n");
}

void clear(CPU* cpu)
{
    cpu->clear();
}

void ret(CPU* cpu)
{
    cpu->ret();
}

void jmp(CPU *cpu, uint16_t instr)
{
    cpu->set_PC(instr & 0xFFF);
}

void call(CPU *cpu, uint16_t instr)
{
    cpu->call_stack[cpu->SP] = cpu->get_PC();
    cpu->set_PC(instr & 0xFFF);
    cpu->SP++;
}

void skpe(CPU *cpu, uint16_t instr)
{
    if (cpu->gpr[(instr >> 8) & 0xF] == (instr & 0xFF))
        cpu->set_PC(cpu->get_PC() + 2);
}

void skpn(CPU *cpu, uint16_t instr)
{
    if (cpu->gpr[(instr >> 8) & 0xF] != (instr & 0xFF))
        cpu->set_PC(cpu->get_PC() + 2);
}

void ld_imm(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] = instr & 0xFF;
}

void add_imm(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] += instr & 0xFF;
}

void ld_reg(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] = cpu->gpr[(instr >> 4) & 0xF];
}

void and_reg(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] &= cpu->gpr[(instr >> 4) & 0xF];
}

void xor_reg(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] ^= cpu->gpr[(instr >> 4) & 0xF];
}

void adc_reg(CPU *cpu, uint16_t instr)
{
    uint16_t result = cpu->gpr[(instr >> 8) & 0xF] + cpu->gpr[(instr >> 4) & 0xF];
    cpu->gpr[0xF] = result > 0xFF;
    cpu->gpr[(instr >> 8) & 0xF] = result & 0xFF;
}

void sbc_reg(CPU *cpu, uint16_t instr)
{
    uint8_t x = cpu->gpr[(instr >> 8) & 0xF];
    uint8_t y = cpu->gpr[(instr >> 4) & 0xF];
    cpu->gpr[0xF] = x >= y;
    cpu->gpr[(instr >> 8) & 0xF] = x - y;
}

void srl(CPU *cpu, uint16_t instr)
{
    uint8_t y = cpu->gpr[(instr >> 4) & 0xF];
    cpu->gpr[0xF] = y & 0x1;
    cpu->gpr[(instr >> 8) & 0xF] = y >> 1;
}

void sll(CPU *cpu, uint16_t instr)
{
    uint8_t y = cpu->gpr[(instr >> 4) & 0xF];
    cpu->gpr[0xF] = y >> 7;
    cpu->gpr[(instr >> 8) & 0xF] = y << 1;
}

void ld_i(CPU *cpu, uint16_t instr)
{
    cpu->I = instr & 0xFFF;
}

void rand_cpu(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] = rand() & instr & 0xFF;
}

void draw_cpu(CPU *cpu, uint16_t instr)
{
    uint8_t x = cpu->gpr[(instr >> 8) & 0xF];
    uint8_t y = cpu->gpr[(instr >> 4) & 0xF];
    uint8_t rows = instr & 0xF;
    uint8_t data;

    bool bit_flipped_off = false;
    for (int i = 0; i < rows; i++)
    {
        if (cpu->I + i >= 4096)
        {
            printf("Bad I + i: $%04X\n", cpu->I + i);
            exit(1);
        }
        data = cpu->memory[cpu->I + i];
        for (int j = 0; j < 8; j++)
        {
            uint32_t addr = x + j + ((y + i) * 64);
            //printf("Addr: $%08X\n", addr);
            if (addr >= 64 * 32)
                continue;

            if (data & (1 << (7 - j)))
            {
                cpu->framebuffer[addr] ^= 0xFFFFFF;
                if ((cpu->framebuffer[addr] & 0xFFFFFF) == 0)
                    bit_flipped_off = true;
            }
        }
    }

    cpu->gpr[0xF] = bit_flipped_off;
}

void skpn_key(CPU *cpu, uint16_t instr)
{
    if (true)
        cpu->set_PC(cpu->get_PC() + 2);
}

void ld_reg_delay(CPU *cpu, uint16_t instr)
{
    cpu->gpr[(instr >> 8) & 0xF] = cpu->get_delay();
}

void ld_delay_reg(CPU *cpu, uint16_t instr)
{
    cpu->set_delay(cpu->gpr[(instr >> 8) & 0xF]);
}

void add_i(CPU *cpu, uint16_t instr)
{
    cpu->I += cpu->gpr[(instr >> 8) & 0xF];
}

void sprite(CPU *cpu, uint16_t instr)
{
    cpu->I = cpu->gpr[(instr >> 8) & 0xF] * 5;
}

void bcd(CPU *cpu, uint16_t instr)
{
    uint8_t x = cpu->gpr[(instr >> 8) & 0xF];
    uint8_t digit0 = 0, digit1 = 0, digit2 = 0;
    digit0 = x & 0xF;
    if (digit0 > 9)
    {
        digit1++;
        digit0 -= 10;
    }

    printf("Hex: $%02X Digits: %d%d%d\n", x, digit2, digit1, digit0);
    cpu->memory[cpu->I + 2] = digit0;
    cpu->memory[cpu->I + 1] = digit1;
    cpu->memory[cpu->I] = digit2;
}

void reg_to_mem(CPU *cpu, uint16_t instr)
{
    memcpy(cpu->memory + cpu->I, cpu->gpr, ((instr >> 8) & 0xF) + 1);
    cpu->I += ((instr >> 8) & 0xF) + 1;
}

void mem_to_reg(CPU *cpu, uint16_t instr)
{
    memcpy(cpu->gpr, cpu->memory + cpu->I, ((instr >> 8) & 0xF) + 1);
    cpu->I += ((instr >> 8) & 0xF) + 1;
}

};
