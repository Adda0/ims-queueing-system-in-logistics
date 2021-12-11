//===============================================================================================================
// File:        generator.cpp
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#include "generator.h"
#include "orders.h"

unsigned Generator::orderSpan(ORDER_SPAN_DEFAULT);
unsigned Generator::orderSpanRushHour(ORDER_SPAN_RUSH_DEFAULT);

void Generator::Behavior()
{
    auto *order = new Order();
    order->Activate();

    if (Time >= RUSH_HOUR_START && Time < RUSH_HOUR_END)    // rush hour
    {
        Activate(Time + Exponential(orderSpanRushHour));
    }
    else if (Time <= OPENING_HOURS)  // normal delivery time
    {
        Activate(Time + Exponential(orderSpan));
    }
}

bool Generator::IsIncreasedTraffic(unsigned time)
{
    if (time >= INCREASED_TRAFFIC_START && time < INCREASED_TRAFFIC_END)
    {
        return true;
    }

    return false;
}

void Generator::SetOrderSpans(unsigned orderSpanPar, unsigned orderSpanRushHourPar)
{
    orderSpan = orderSpanPar;
    if (orderSpanRushHourPar == 0) // Span between orders during rush hour not set, calculated as half of the normal span.
    {
        orderSpanRushHour = orderSpanPar >> 1;
    }
}
