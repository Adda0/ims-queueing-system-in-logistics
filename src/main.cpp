
#include "main.h"

int main(int argc, char **argv)
{
    RandomSeed(time(nullptr));
    Program::Run(argc, argv);
    return 0;
}
