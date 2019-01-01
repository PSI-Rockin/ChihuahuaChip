#include "emitterx64.hpp"

Emitterx64::Emitterx64(JitCache* cache) : cache(cache)
{

}

void Emitterx64::rexw_rm(REG_64 rm)
{
    uint8_t rex = 0x48;
    if (rm & 0x8)
        rex |= 0x1;
    cache->write8(rex);
}

void Emitterx64::rexw_r_rm(REG_64 reg, REG_64 rm)
{
    uint8_t rex = 0x48;
    if (reg & 0x8)
        rex |= 0x4;
    if (rm & 0x8)
        rex |= 0x1;
    cache->write8(rex);
}

void Emitterx64::modrm(uint8_t mod, uint8_t reg, uint8_t rm)
{
    cache->write8(((mod & 0x3) << 6) | ((reg & 0x7) << 3) | (rm & 0x7));
}

void Emitterx64::xADD32_MI32_MEM(uint32_t imm, REG_64 indir_dest)
{
    cache->write8(0x81);
    modrm(0, 0, indir_dest);
    cache->write32(imm);
}

void Emitterx64::xADD64_MI8(uint8_t imm, REG_64 dest)
{
    rexw_rm(dest);
    cache->write8(0x83);
    modrm(0b11, 0, dest);
    cache->write8(imm);
}

void Emitterx64::xMOV32_MI(uint32_t imm, REG_64 dest)
{
    cache->write8(0xC7);
    modrm(0, 0, dest);
    cache->write32(imm);
}

void Emitterx64::xMOV32_MI_MEM(uint32_t imm, REG_64 indir_dest)
{
    cache->write8(0xC7);
    modrm(0, 0, indir_dest);
    cache->write32(imm);
}

void Emitterx64::xMOV64_MR(REG_64 source, REG_64 dest)
{
    rexw_r_rm(source, dest);
    cache->write8(0x89);
    modrm(0b11, source, dest);
}

void Emitterx64::xMOV64_OI(uint64_t imm, REG_64 dest)
{
    rexw_rm(dest);
    cache->write8(0xB8 + (dest & 0x7));
    cache->write64(imm);
}

void Emitterx64::xCALL(uint64_t func)
{
    int offset = func;
    offset -= (uint64_t)cache->get_current_addr();
    //printf("Offset: %llX - %llX = $%08X\n", func, cache->get_current_addr(), offset);
    cache->write8(0xE8);
    cache->write32(offset - 5);
}

void Emitterx64::xCALL_INDIR(REG_64 source)
{
    cache->write8(0xFF);
    modrm(0b11, 2, source);
}

void Emitterx64::xPUSH(REG_64 reg)
{
    cache->write8(0xFF);
    modrm(0b11, 6, reg);
}

void Emitterx64::xPOP(REG_64 reg)
{
    cache->write8(0x8F);
    modrm(0b11, 0, reg);
}

void Emitterx64::xRET()
{
    cache->write8(0xC3);
}
