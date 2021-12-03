#pragma once
#include <simlib.h>
#include <iostream>
#include <vector>
#include <queue>

#include "drivers.h"

using namespace std;

enum DriverType
{
    Unset,
    BikeRider,
    CarDriver,
};

class Order : public Process
{
private:
    static Queue _driversQueue;
    static queue<Order *> *_kitchenQueue;
    static vector<unsigned> _ordersDeliverySpans;
    static DriverType _driver;
    static unsigned _travelMin;
    static unsigned _travelMax;
    static double _bikesToCars;
    static void PrintAverage();
    static void PrintDelayed(unsigned cutoff);

    double _start = 0;
    queue<Order *> *_deliveryQueue = nullptr;
    DriverType _myDriver = Unset;
    void Behavior();

public:
    static void SetTravelTimes(unsigned min, unsigned max);
    static void SetBikesToCars(unsigned bikes, unsigned cars);
    static void Stats(unsigned cuttoff);

    void StartDelivery(queue<Order *> *queue);
};
