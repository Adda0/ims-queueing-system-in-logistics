#include "generator.h"
#include "orders.h"

void Generator::Behavior()
{
    Order *order = new Order();
    order->Activate();

    if (Time >= 7600 && Time < 15200)
    {
        Activate(Time + Exponential(120));
    }
    else if (Time <= 28800)
    {
        Activate(Time + Exponential(300));
    }
}
