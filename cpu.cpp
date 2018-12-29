#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "cpu.hpp"

#define printf(fmt, ...)(0)

CPU::CPU(uint8_t* memory, uint32_t* framebuffer) : memory(memory), framebuffer(framebuffer)
{

}

void CPU::reset()
{
    srand(time(NULL));
    PC = 0x200;
    delay = 0;
    SP = 0;
}

void CPU::run(int cycles)
{
    while (cycles > 0)
    {
        cycles--;
        uint16_t instr = memory[PC + 1];
        instr |= ((uint16_t)memory[PC]) << 8;

        printf("[$%04X] $%04X ", PC, instr);

        PC += 2;

        switch (instr & 0xF000)
        {
            case 0x0000:
                switch (instr & 0xFFF)
                {
                    case 0x0E0:
                        printf("CLEAR");
                        for (int i = 0; i < 64 * 32; i++)
                            framebuffer[i] = 0xFF000000;
                        break;
                    case 0x0EE:
                        printf("RET");
                        SP--;
                        PC = call_stack[SP];
                        break;
                    default:
                        printf("Unrecognized 0000 instruction $%04X\n", instr & 0xFFF);
                        exit(1);
                }
                break;
            case 0x1000:
                printf("JMP $%04X", instr & 0xFFF);
                PC = instr & 0xFFF;
                break;
            case 0x2000:
                printf("CALL $%04X", instr & 0xFFF);
                call_stack[SP] = PC;
                PC = instr & 0xFFF;
                SP++;
                break;
            case 0x3000:
                printf("SKPE V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
                if (gpr[(instr >> 8) & 0xF] == (instr & 0xFF))
                    PC += 2;
                break;
            case 0x4000:
                printf("SKPN V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
                if (gpr[(instr >> 8) & 0xF] != (instr & 0xFF))
                    PC += 2;
                break;
            case 0x6000:
                printf("LD V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
                gpr[(instr >> 8) & 0xF] = instr & 0xFF;
                break;
            case 0x7000:
                printf("ADD V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
                gpr[(instr >> 8) & 0xF] += instr & 0xFF;
                break;
            case 0x8000:
                switch (instr & 0xF)
                {
                    case 0x0:
                        printf("LD V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                        gpr[(instr >> 8) & 0xF] = gpr[(instr >> 4) & 0xF];
                        break;
                    case 0x2:
                        printf("AND V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                        gpr[(instr >> 8) & 0xF] &= gpr[(instr >> 4) & 0xF];
                        break;
                    case 0x3:
                        printf("XOR V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                        gpr[(instr >> 8) & 0xF] ^= gpr[(instr >> 4) & 0xF];
                        break;
                    case 0x4:
                        printf("ADC V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    {
                        uint16_t result = gpr[(instr >> 8) & 0xF] + gpr[(instr >> 4) & 0xF];
                        gpr[0xF] = result > 0xFF;
                        gpr[(instr >> 8) & 0xF] = result & 0xFF;
                    }
                        break;
                    case 0x5:
                        printf("SBC V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    {
                        uint8_t x = gpr[(instr >> 8) & 0xF];
                        uint8_t y = gpr[(instr >> 4) & 0xF];
                        gpr[0xF] = x >= y;
                        gpr[(instr >> 8) & 0xF] = x - y;
                    }
                        break;
                    case 0x6:
                        printf("SRL V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    {
                        uint8_t y = gpr[(instr >> 4) & 0xF];
                        gpr[0xF] = y & 0x1;
                        gpr[(instr >> 8) & 0xF] = y >> 1;
                    }
                        break;
                    case 0xE:
                        printf("SLL V%01X, V%01X", (instr >> 8) & 0xF, (instr >> 4) & 0xF);
                    {
                        uint8_t y = gpr[(instr >> 4) & 0xF];
                        gpr[0xF] = y >> 7;
                        gpr[(instr >> 8) & 0xF] = y << 1;
                    }
                        break;
                    default:
                        printf("Unrecognized 8000 instruction $%04X\n", instr & 0xF);
                        exit(1);
                }
                break;
            case 0xA000:
                printf("LD I, $%04X", instr & 0xFFF);
                I = instr & 0xFFF;
                break;
            case 0xC000:
                printf("RAND V%01X, $%02X", (instr >> 8) & 0xF, instr & 0xFF);
                gpr[(instr >> 8) & 0xF] = rand() & instr & 0xFF;
                break;
            case 0xD000:
                printf("DRAW V%01X, V%01X, $%02X", (instr >> 8) & 0xF, (instr >> 4) & 0xF, instr & 0xF);
            {
                uint8_t x = gpr[(instr >> 8) & 0xF];
                uint8_t y = gpr[(instr >> 4) & 0xF];
                uint8_t rows = instr & 0xF;
                uint8_t data;

                bool bit_flipped_off = false;
                for (int i = 0; i < rows; i++)
                {
                    data = memory[I + i];
                    for (int j = 0; j < 8; j++)
                    {
                        uint32_t addr = x + j + ((y + i) * 64);
                        if (addr > (64 * 32))
                            continue;

                        if (data & (1 << (7 - j)))
                        {
                            framebuffer[addr] ^= 0xFFFFFF;
                            if ((framebuffer[addr] & 0xFFFFFF) == 0)
                                bit_flipped_off = true;
                        }
                    }
                }

                gpr[0xF] = bit_flipped_off;
            }
                break;
            case 0xE000:
                switch (instr & 0xFF)
                {
                    case 0xA1:
                        printf("SKPN V%01X, KEYPAD", (instr >> 8) & 0xF);
                        if (true)
                            PC += 2;
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
                        gpr[(instr >> 8) & 0xF] = delay;
                        break;
                    case 0x15:
                        printf("LD DELAY, V%01X", (instr >> 8) & 0xF);
                        delay = gpr[(instr >> 8) & 0xF];
                        break;
                    case 0x18:
                        printf("LD SOUND, V%01X", (instr >> 8) & 0xF);
                        break;
                    case 0x1E:
                        printf("ADD I, V%01X", (instr >> 8) & 0xF);
                        I += gpr[(instr >> 8) & 0xF];
                        break;
                    case 0x29:
                        printf("SPRITE I, V%01X", (instr >> 8) & 0xF);
                        I = gpr[(instr >> 8) & 0xF] * 5;
                        break;
                    case 0x33:
                        printf("BCD V%01X\n", (instr >> 8) & 0xF);
                    {
                        uint8_t x = gpr[(instr >> 8) & 0xF];
                        uint8_t digit0 = 0, digit1 = 0, digit2 = 0;
                        digit0 = x & 0xF;
                        if (digit0 > 9)
                        {
                            digit1++;
                            digit0 -= 10;
                        }

                        printf("Hex: $%02X Digits: %d%d%d\n", x, digit2, digit1, digit0);
                        memory[I + 2] = digit0;
                        memory[I + 1] = digit1;
                        memory[I] = digit2;
                    }
                        break;
                    case 0x55:
                        printf("MEMCPY(MEM[I], GPR, %d)", ((instr >> 8) & 0xF) + 1);
                        memcpy(memory + I, gpr, ((instr >> 8) & 0xF) + 1);
                        I += ((instr >> 8) & 0xF) + 1;
                        break;
                    case 0x65:
                        printf("MEMCPY(GPR, MEM[I], %d)", ((instr >> 8) & 0xF) + 1);
                        memcpy(gpr, memory + I, ((instr >> 8) & 0xF) + 1);
                        I += ((instr >> 8) & 0xF) + 1;
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
}

void CPU::dec_delay()
{
    if (delay)
        delay--;
}
