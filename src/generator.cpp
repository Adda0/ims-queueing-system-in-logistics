#include "generator.h"
#include "orders.h"

unsigned Generator::orderSpan(10);
unsigned Generator::orderSpanRushHour(Generator::orderSpan >> 1);

void Generator::Behavior()
{
    Order *order = new Order();
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

bool Generator::IsIncreasedTrafic(unsigned time)
{
    if (time >= INCREASED_TRAFIC_START && time < INCREASED_TRAFIC_END)
    {
        return true;
    }

    return false;
}

void Generator::SetOrderSpans(unsigned orderSpan, unsigned orderSpanRushHour)
{
    orderSpan = orderSpan;
    if (orderSpanRushHour == 0) // span beween orders during rush hour not set, calculated as half of the normal span
    {
        orderSpanRushHour = orderSpan >> 1;
    }
}
