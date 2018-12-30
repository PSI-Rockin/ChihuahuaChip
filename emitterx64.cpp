#include "emitterx64.hpp"

Emitterx64::Emitterx64(JitCache* cache) : cache(cache)
{

}

uint8_t Emitterx64::modrm(uint8_t mod, uint8_t reg, uint8_t rm)
{
    return ((mod & 0x3) << 6) | ((reg & 0x7) << 3) | (rm & 0x7);
}

void Emitterx64::xADD64_MI8(uint8_t imm, REG_64 dest)
{
    uint8_t rex = 0x48;
    cache->write8(rex);
    cache->write8(0x83);
    cache->write8(modrm(0b11, 0, dest));
    cache->write8(imm);
}

void Emitterx64::xMOV64_MR(REG_64 source, REG_64 dest)
{
    uint8_t rex = 0x48;
    cache->write8(rex);
    cache->write8(0x89);
    cache->write8(modrm(0b11, source, dest));
}

void Emitterx64::xRET()
{
    cache->write8(0xC3);
}
