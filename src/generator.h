#pragma once

#include <simlib.h>
#include <vector>

#include "orders.h"

class Generator : public Event
{
private:
    void Behavior();
};
