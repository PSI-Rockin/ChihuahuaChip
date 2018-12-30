#ifndef JITCACHE_HPP
#define JITCACHE_HPP
#include <cstdint>
#include <vector>

struct JitBlock
{
    uint16_t start_pc;
    uint8_t* block_mem;
};

class JitCache
{
    private:
        constexpr static int BLOCK_SIZE = 1024;

        std::vector<JitBlock> blocks;
    public:
        JitCache();
        ~JitCache();

        uint8_t* alloc_block();
};

#endif // JITCACHE_HPP
