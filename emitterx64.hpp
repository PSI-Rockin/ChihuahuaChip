#ifndef EMITTERX64_HPP
#define EMITTERX64_HPP
#include "jitcache.hpp"

enum REG_64
{
    EAX = 0, RAX = 0,
    ECX = 1, RCX = 1,
    EDX = 2, RDX = 2,
    EBX = 3, RBX = 3,
    ESP = 4, RSP = 4,
    EBP = 5, RBP = 5,
    ESI = 6, RSI = 6,
    EDI = 7, RDI = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15
};

class Emitterx64
{
    private:
        JitCache* cache;

        void rexw_rm(REG_64 rm);
        void rexw_r_rm(REG_64 reg, REG_64 rm);
        void modrm(uint8_t mod, uint8_t reg, uint8_t rm);
    public:
        Emitterx64(JitCache* cache);

        void xADD32_MI32_MEM(uint32_t imm, REG_64 indir_dest);
        void xADD64_MI8(uint8_t imm, REG_64 dest);

        void xMOV32_MI(uint32_t imm, REG_64 dest);
        void xMOV32_MI_MEM(uint32_t imm, REG_64 indir_dest);
        void xMOV64_MR(REG_64 source, REG_64 dest);
        void xMOV64_OI(uint64_t imm, REG_64 dest);

        void xCALL(uint64_t func);
        void xCALL_INDIR(REG_64 source);

        void xPUSH(REG_64 reg);
        void xPOP(REG_64 reg);
        void xRET();
};

#endif // EMITTERX64_HPP
