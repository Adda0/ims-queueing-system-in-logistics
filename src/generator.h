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
#define RUSH_HOUR_START          120           // 120 min (2 hours) after opening, 12:00
#define RUSH_HOUR_END            270           // 270 min (4.5 hours) after opening, i.e. the rush hour lasts for 2.5 hours, to 14:30 
#define INCREASED_TRAFIC_START   330           // 330 min (5.5 hours) after opening, 15:30
#define INCREASED_TRAFIC_END     480           // 480 min (8 hours) after opening, i.e increased trafic lasts for 2.5 hours, to 18:00

#define ORDER_SPAN_DEFAULT 10
#define ORDER_SPAN_RUSH_DEFAULT 5

class Generator : public Event
{
private:

    void Behavior();

public:
    static unsigned orderSpan;
    static unsigned orderSpanRush;

    static bool IsIncreasedTrafic(unsigned time);
};
