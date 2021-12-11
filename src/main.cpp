//===============================================================================================================
// File:        main.cpp
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#include "main.h"

int main(int argc, char **argv)
{
    RandomSeed(time(nullptr));
    Program::Run(argc, argv);
    return 0;
}
