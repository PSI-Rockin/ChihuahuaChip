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

    b.block_mem = (uint8_t*)mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (!b.block_mem)
        return -1;

    b.start_pc = pc;
    b.block_start = b.block_mem;
    blocks.push_back(b);

    current_block = blocks.size() - 1;

    return current_block;
}

void JitCache::test()
{
    func f = reinterpret_cast<func>(blocks[current_block].block_start);
    printf("blorp: %d\n", f(4));

    uint8_t* pointer = blocks[current_block].block_start;
    while (pointer != blocks[current_block].block_mem)
    {
        printf("%02X", *pointer);
        pointer++;
    }
}

void JitCache::write8(uint8_t value)
{
    JitBlock* b = &blocks[current_block];
    *b->block_mem = value;
    b->block_mem++;
}
