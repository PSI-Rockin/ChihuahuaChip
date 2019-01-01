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
        uint8_t* addresses[4096];

        void print_cache();
    public:
        JitCache();
        ~JitCache();

        int alloc_block(uint16_t pc);
        int set_block_rx(uint16_t pc);
        int find_block(uint16_t pc);
        uint8_t* get_current_block_start();
        uint8_t* get_current_addr();
        uint8_t* get_addr(uint16_t pc);

        void write8(uint8_t value);
        void write16(uint16_t value);
        void write32(uint32_t value);
        void write64(uint64_t value);
};

#endif // JITCACHE_HPP
