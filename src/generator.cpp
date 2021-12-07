#include "generator.h"
#include "orders.h"

unsigned Generator::_orderSpan(0);
unsigned Generator::_orderSpanRushHour(0);

void Generator::Behavior()
{
    Order *order = new Order();
    order->Activate();

    if (Time >= RUSH_HOUR_START && Time < RUSH_HOUR_END)    // rush hour
    {
        Activate(Time + Exponential(_orderSpanRushHour));
    }
    else if (Time <= OPENING_HOURS)  // normal delivery time
    {
        Activate(Time + Exponential(_orderSpan));
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
    _orderSpan = orderSpan;
    if (orderSpanRushHour == 0) // span beween orders during rush hour not set, calculated as half of the normal span
    {
        _orderSpanRushHour = orderSpan >> 1;
    }
}
