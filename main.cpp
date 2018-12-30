#include <QApplication>

#include <fstream>
#include "emulator.hpp"
#include "displaywindow.hpp"
#include "jitcache.hpp"

using namespace std;

typedef long (*func)(long);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Input a ROM name\n");
        return 1;
    }

    ifstream file(argv[1], std::ios::binary);
    if (!file.is_open())
    {
        printf("Unable to open %s\n", argv[1]);
        return 1;
    }

    printf("Opened %s successfully\n", argv[1]);
    int file_size = 0x503;
    uint8_t* ROM = new uint8_t[file_size];
    file.read((char*)ROM, file_size);
    file.close();
    Emulator e;
    e.load_rom(ROM, file_size);
    e.reset();

    chrono::system_clock::time_point old_frametime = chrono::system_clock::now();
    double FPS;

    int max_frames = 1000;
    int frames = 0;

    JitCache cache;
    uint8_t* block = cache.alloc_block();

    unsigned char code[] =
    {
      0x48, 0x89, 0xf8,                   // mov %rdi, %rax
      0x48, 0x83, 0xc0, 0x08,             // add $8, %rax
      0xc3                                // ret
    };
    memcpy(block, code, sizeof(code));

    func f = reinterpret_cast<func>(block);
    int blorp = f(24);
    printf("Blorp: %d\n", blorp);

    /*while (frames < max_frames)
    {
        e.run();
        frames++;
        if (frames % 60 == 59)
        {
            chrono::system_clock::time_point now = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = now - old_frametime;
            old_frametime = chrono::system_clock::now();
            FPS = 60 / elapsed_seconds.count();
            printf("FPS: %f\n", FPS);
        }
    }*/
    return 0;
}
