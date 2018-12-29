#include <QApplication>

#include <fstream>
#include "emulator.hpp"
#include "displaywindow.hpp"

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
    int file_size = 0xF6;
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

    while (frames < max_frames)
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
    }
    return 0;
}
