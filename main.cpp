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
    uint8_t* ROM = new uint8_t[0x503];
    file.read((char*)ROM, 0x503);
    file.close();
    Emulator e;
    e.load_rom(ROM, 0x503);
    e.reset();
    e.run();
    return 0;
}
