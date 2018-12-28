#include <fstream>
#include "emulator.hpp"

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
    e.run();
    return 0;
}
