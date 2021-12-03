#pragma once

#include <getopt.h>
#include <string>
#include <limits>
#include <iostream>
#include <simlib.h>

#include "drivers.h"
#include "orders.h"
#include "generator.h"

#define OPENING_HOURS 28800             // 28800 s, which is 8 hours
#define ADDITIONAL_DELIVERY_TIME 3600   // 3600 s, which is 1 hour

using namespace std;

class Program
{
private:
    static unsigned _cars;
    static unsigned _bikes;
    static unsigned _orderSpan;
    static unsigned _deliveryTime;
    static unsigned _orderSpanPeak;
    static unsigned _travel;
    static unsigned _travelMin;
    static unsigned _travelMax;
    
    static void ParseArguments(int argc, char **argv);
    static void HelpMsg();

public:
    static void Run(int argc, char **argv);
};
