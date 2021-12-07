#pragma once

#include <simlib.h>
#include <vector>

#include "orders.h"


#define OPENING_HOURS            720           // 720 min (12 hours)
#define ADDITIONAL_DELIVERY_TIME 60            // 60 min (1 hour)
#define RUSH_HOUR_START          120           // 120 min (2 hours) after opening
#define RUSH_HOUR_END            270           // 270 min (4.5 hours) after opening, i.e. the rush hour lasts for 2.5 hours
#define INCREASED_TRAFIC_START   330           // 330 min (5.5 hours) after opening
#define INCREASED_TRAFIC_END     480           // 480 min (8 hours) after opening, i.e increased trafic lasts for 2.5 hours

class Generator : public Event
{
private:
    static unsigned _orderSpan;
    static unsigned _orderSpanRushHour;

    void Behavior();

public:
    static bool IsIncreasedTrafic(unsigned time);
    static void SetOrderSpans(unsigned orderSpan, unsigned orderSpanRushHour);
};
