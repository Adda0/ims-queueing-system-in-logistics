//===============================================================================================================
// File:        program.h
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#pragma once

#include <getopt.h>
#include <string>
#include <limits>
#include <iostream>
#include <simlib.h>

#include "drivers.h"
#include "orders.h"
#include "generator.h"

using namespace std;

#define NUMBER_OF_CARS_DEFAULT 4
#define NUMBER_OF_BIKES_DEFAULT 1

class Program
{
private:
    static unsigned _cars;
    static unsigned _bikes;

    static void ParseArguments(int argc, char **argv);
    static void HelpMessage();

public:
    static void Run(int argc, char **argv);
};
