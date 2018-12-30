#include <sys/mman.h>
#include <cstdint>
#include <cstdio>
#include "jitcache.hpp"

JitCache::JitCache()
{
    blocks.reserve(1024 * 4);
}

JitCache::~JitCache()
{

}

uint8_t* JitCache::alloc_block()
{
    JitBlock b;

    b.block_mem = (uint8_t*)mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (!b.block_mem)
        return nullptr;

    blocks.push_back(b);

    return b.block_mem;
}
