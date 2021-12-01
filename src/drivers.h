#pragma once

#include "simlib.h"

class Drivers
{
private:
    static Store _store;
    static Queue _queue;

public:
    static void init_drivers(unsigned long capacity);

    static Store *get_drivers();
};
