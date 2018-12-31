#include <sys/mman.h>
#include <cstdint>
#include <cstdio>
#include "jitcache.hpp"

typedef long (*func)(long);

JitCache::JitCache()
{
    blocks.reserve(1024 * 4);
    current_block = -1;
}

JitCache::~JitCache()
{

}

int JitCache::alloc_block(uint16_t pc)
{
    JitBlock b;

    b.block_mem = (uint8_t*)mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (!b.block_mem)
        return -1;

    b.start_pc = pc;
    b.block_start = b.block_mem;
    blocks.push_back(b);

    current_block = blocks.size() - 1;

    return current_block;
}

int JitCache::set_block_rx(uint16_t pc)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i].start_pc == pc)
        {
            if (mprotect(blocks[i].block_start, BLOCK_SIZE, PROT_READ | PROT_EXEC) == -1)
                return -1;
            break;
        }
    }
    return 0;
}

int JitCache::find_block(uint16_t pc)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i].start_pc == pc)
        {
            current_block = i;
            return current_block;
        }
    }
    return -1;
}

uint8_t* JitCache::get_current_block_start()
{
    return blocks[current_block].block_start;
}

void JitCache::write8(uint8_t value)
{
    JitBlock* b = &blocks[current_block];
    *b->block_mem = value;
    b->block_mem++;
}

void JitCache::write16(uint16_t value)
{
    JitBlock* b = &blocks[current_block];
    *(uint16_t*)b->block_mem = value;
    b->block_mem += 2;
}

void JitCache::write32(uint32_t value)
{
    JitBlock* b = &blocks[current_block];
    *(uint32_t*)b->block_mem = value;
    b->block_mem += 4;
}

void JitCache::write64(uint64_t value)
{
    JitBlock* b = &blocks[current_block];
    *(uint64_t*)b->block_mem = value;
    b->block_mem += 8;
}
