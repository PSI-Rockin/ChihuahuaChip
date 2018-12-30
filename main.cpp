#include <QApplication>

#include <fstream>
#include "emulator.hpp"
#include "emitterx64.hpp"
#include "displaywindow.hpp"
#include "jitcache.hpp"

using namespace std;

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
    cache.alloc_block(0x200);
    Emitterx64 emitter(&cache);
    emitter.xMOV64_MR(REG_64::RDI, REG_64::RAX);
    emitter.xADD64_MI8(0x10, REG_64::RAX);
    emitter.xRET();

    cache.test();

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
