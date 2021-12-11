//===============================================================================================================
// File:        generator.cpp
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#include "generator.h"
#include "orders.h"

unsigned Generator::orderSpan(ORDER_SPAN_DEFAULT);
unsigned Generator::orderSpanRush(ORDER_SPAN_RUSH_DEFAULT);

void Generator::Behavior()
{
    Order *order = new Order();
    order->Activate();

    if (Time >= RUSH_HOUR_START && Time < RUSH_HOUR_END)    // rush hour
    {
        Activate(Time + Exponential(orderSpanRush));
    }
    else if (Time <= OPENING_HOURS)  // normal delivery time
    {
        Activate(Time + Exponential(orderSpan));
    }
}

bool Generator::IsIncreasedTrafic(unsigned time)
{
    if (time >= INCREASED_TRAFIC_START && time < INCREASED_TRAFIC_END)
    {
        return true;
    }

    return false;
}

