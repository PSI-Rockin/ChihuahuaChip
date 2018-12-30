#ifndef JITCACHE_HPP
#define JITCACHE_HPP
#include <cstdint>
#include <vector>

struct JitBlock
{
    uint16_t start_pc;
    uint8_t* block_start;
    uint8_t* block_mem;
};

class JitCache
{
    private:
        constexpr static int BLOCK_SIZE = 1024;

        int current_block;

        std::vector<JitBlock> blocks;
    public:
        JitCache();
        ~JitCache();

        int alloc_block(uint16_t pc);
        void test();

        void write8(uint8_t value);
};

#endif // JITCACHE_HPP
