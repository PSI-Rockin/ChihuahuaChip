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
    if (cache.find_block(cpu->get_PC()) == -1)
        recompile_block();

    return cache.get_current_block_start();
}

void run()
{
    __asm__ (
        "pushq %rax\n"
        "pushq %rcx\n"
        "pushq %rdx\n"
        "pushq %rbx\n"

        //No point in saving RSP, as we don't use it...
        //Same for RBP, as it already gets saved by the function

        "pushq %rsi\n"
        "pushq %rdi\n"
        /*"pushq %r8\n"
        "pushq %r9\n"
        "pushq %r10\n"
        "pushq %r11\n"
        "pushq %r12\n"
        "pushq %r13\n"
        "pushq %r14\n"
        "pushq %r15\n"*/

        "callq __ZN12JitTranslate10exec_blockEv\n"
        "callq *%rax\n"

        /*"popq %r15\n"
        "popq %r14\n"
        "popq %r13\n"
        "popq %r12\n"
        "popq %r11\n"
        "popq %r10\n"
        "popq %r9\n"
        "popq %r8\n"*/
        "popq %rdi\n"
        "popq %rsi\n"
        "popq %rbx\n"
        "popq %rdx\n"
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

    while (!branch_hit)
    {
        uint16_t instr = cpu->read16(pc);
        //printf("[JIT] PC: $%04X\n", start_pc);
        switch (instr & 0xF000)
        {
            case 0x0000:
                switch (instr & 0xFFF)
                {
                    case 0xEE:
                        branch_hit = true;
                        break;
                }
                break;
            case 0x1000:
            case 0x2000:
            case 0x3000:
            case 0x4000:
            case 0xE000:
                //jump(instr & 0xFFF);
                branch_hit = true;
                break;
        }

        emitter.xMOV64_OI((uint64_t)cpu, REG_64::RDI);
        emitter.xMOV64_OI(instr, REG_64::RSI);
        emitter.xMOV64_OI((uint64_t)Interpreter::interpret, REG_64::RBX);
        emitter.xCALL_INDIR(REG_64::RBX);

        pc += 2;
        instructions++;
    }

    printf("[JIT] End block: $%04X\n", pc - 2);

    emitter.xMOV64_OI((uint64_t)&cpu->cycles, REG_64::RBX);
    emitter.xADD32_MI32_MEM(instructions, REG_64::RBX);
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
