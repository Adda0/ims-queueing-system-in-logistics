//===============================================================================================================
// File:        generator.h
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#pragma once

#include <simlib.h>
#include <vector>

#include "orders.h"


#define OPENING_HOURS            720           // 720 min (12 hours), 10:00 - 22:00
#define ADDITIONAL_DELIVERY_TIME 120           // 120 min (2 hours), additional simulation time allowing delivery of last orders.
#define RUSH_HOUR_START          90            // 90 min (1.5 hours) after opening, 11:30
#define RUSH_HOUR_END            240           // 240 min (4 hours) after opening, i.e. the rush hour lasts for 2.5 hours, to 14:00
#define INCREASED_TRAFFIC_START   330           // 330 min (5.5 hours) after opening, 15:30
#define INCREASED_TRAFFIC_END     480           // 480 min (8 hours) after opening, i.e increased trafic lasts for 2.5 hours, to 18:00

#define ORDER_SPAN_DEFAULT 10
#define ORDER_SPAN_RUSH_DEFAULT 5

class Generator : public Event
{
private:

    void Behavior();

public:
    static unsigned orderSpan;
    static unsigned orderSpanRushHour;

    static bool IsIncreasedTraffic(unsigned time);
    static void SetOrderSpans(unsigned orderSpanPar, unsigned orderSpanRushHourPar);
};
