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
