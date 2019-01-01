#include <cstdio>
#include <cstdlib>
#include "jittranslate.hpp"
#include "interpreter.hpp"

namespace JitTranslate
{

CPU* cpu;
JitCache cache;
Emitterx64 emitter(&cache);

bool reg_used[16];
bool reg_locked[16];

void set_cpu(CPU *c)
{
    cpu = c;
}

void reset()
{
    for (int i = 0; i < 16; i++)
    {
        reg_used[i] = false;
        reg_locked[i] = false;
    }
    reg_locked[REG_64::RSP] = true;
}

void flush_gprs()
{
    for (int i = 0; i < 16; i++)
    {

    }
}

uint8_t* exec_block()
{
    uint8_t* addr = cache.get_addr(cpu->PC);
    if (!addr)
    {
        recompile_block();
        return cache.get_addr(cpu->PC);
    }

    return addr;
}

void run()
{
    __asm__ (
        "pushq %rax\n"
        "pushq %rcx\n"
        "pushq %rsi\n"
        "pushq %rdi\n"

        "callq __ZN12JitTranslate10exec_blockEv\n"
        "callq *%rax\n"

        "popq %rdi\n"
        "popq %rsi\n"
        "popq %rcx\n"
        "popq %rax\n"
    );
}

void recompile_block()
{
    uint16_t start_pc = cpu->get_PC();
    cache.alloc_block(start_pc);

    uint16_t pc = start_pc;

    bool branch_hit = false;

    printf("[JIT] Starting recompilation at $%04X\n", start_pc);
    int instructions = 0;

    //Set up the stack frame
    emitter.xPUSH(REG_64::RBP);
    emitter.xMOV64_MR(REG_64::RSP, REG_64::RBP);

    while (!branch_hit)
    {
        bool emit_instr = true;
        uint16_t instr = cpu->read16(pc);

        pc += 2;

        emitter.xMOV64_OI((uint64_t)cpu, REG_64::RDI);

        uint64_t func_addr;
        switch (instr & 0xF000)
        {
            case 0x0000:
                switch (instr & 0xFFF)
                {
                    case 0xE0:
                        emit_instr = false;
                        func_addr = (uint64_t)Interpreter::clear;
                        break;
                    case 0xEE:
                        emit_instr = false;
                        func_addr = (uint64_t)Interpreter::ret;
                        branch_hit = true;
                        break;
                }
                break;
            case 0x1000:
                func_addr = (uint64_t)Interpreter::jmp;
                branch_hit = true;
                break;
            case 0x2000:
                func_addr = (uint64_t)Interpreter::call;
                branch_hit = true;
                break;
            case 0x3000:
                func_addr = (uint64_t)Interpreter::skpe;
                branch_hit = true;
                break;
            case 0x4000:
                func_addr = (uint64_t)Interpreter::skpn;
                branch_hit = true;
                break;
            case 0x6000:
                func_addr = (uint64_t)Interpreter::ld_imm;
                break;
            case 0x7000:
                func_addr = (uint64_t)Interpreter::add_imm;
                break;
            case 0xA000:
                func_addr = (uint64_t)Interpreter::ld_i;
                break;
            case 0xD000:
                func_addr = (uint64_t)Interpreter::draw_cpu;
                break;
            case 0xE000:
                func_addr = (uint64_t)Interpreter::skpn_key;
                branch_hit = true;
                break;
            case 0xF000:
                switch (instr & 0xF)
                {
                    case 0x07:
                        func_addr = (uint64_t)Interpreter::ld_reg_delay;
                        break;
                    case 0x15:
                        func_addr = (uint64_t)Interpreter::ld_delay_reg;
                        break;
                    case 0x1E:
                        func_addr = (uint64_t)Interpreter::add_i;
                        break;
                    default:
                        printf("[JIT] Unknown F000 function $%04X\n", instr);
                        func_addr = (uint64_t)Interpreter::interpret;
                }
                break;
            default:
                printf("[JIT] Unknown function $%04X\n", instr);
                func_addr = (uint64_t)Interpreter::interpret;
        }

        if (emit_instr)
            emitter.xMOV64_OI((uint64_t)instr, REG_64::RSI);

        if (branch_hit)
        {
            emitter.xMOV64_OI((uint64_t)&cpu->PC, REG_64::RAX);
            emitter.xMOV32_MI_MEM(pc, REG_64::RAX);
        }

        emitter.xCALL(func_addr);

        instructions++;
    }

    printf("[JIT] End block: $%04X\n", pc - 2);

    emitter.xMOV64_OI((uint64_t)&cpu->cycles, REG_64::RAX);
    emitter.xADD32_MI32_MEM(instructions, REG_64::RAX);
    emitter.xPOP(REG_64::RBP);
    emitter.xRET();

    cache.set_block_rx(start_pc);
}

void jump(uint16_t addr)
{
    emitter.xMOV64_OI((uint64_t)&cpu->PC, REG_64::RAX);
    emitter.xMOV64_OI((uint64_t)&cpu->PC, REG_64::RAX);
    emitter.xMOV32_MI(addr, REG_64::RAX);
}

};
