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

#define NUMBER_OF_CARS_DEFAULT 3
#define NUMBER_OF_BIKES_DEFAULT 3

class Program
{
private:
    static unsigned _cars;
    static unsigned _bikes;
    
    static void ParseArguments(int argc, char **argv);
    static void HelpMsg();

public:
    static void Run(int argc, char **argv);
};
