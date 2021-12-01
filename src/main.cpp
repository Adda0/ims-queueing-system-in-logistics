
#include "drivers.h"
#include "orders.h"

int main()
{
    Drivers::init_drivers(42);
    Orders order;
    order.Activate(8);
    return 0;
}
