#pragma once
#include <simlib.h>
#include <iostream>
#include <vector>
#include <queue>

#include "drivers.h"
#include "generator.h"

using namespace std;

enum DriverType
{
    Unset,
    BikeRider,
    CarDriver,
};

#define CAR_DRIVER_ORDER_COUNT 4
#define CASH_PAYMENT_PROBABILITY 0.1
#define CASH_PAYMENT_DELAY 2
#define CARD_PAYMENT_PROBABILITY 0.2
#define CARD_PAYMENT_DELAY 1
#define PAIED_IN_ADVANCED_PROBABILITY (1.0 - CASH_PAYMENT_PROBABILITY - CARD_PAYMENT_PROBABILITY)
#define QUALITY_DELAY_TIME 20
#define TRAFIC_JAM_PROBABILITY 0.5
#define EXPIRED_DELIVERY_PAID_PROBABILITY 0.5

class Order : public Process
{
private:
    static Queue _waitingOrders;
    static queue<Order *> *_preparedForPickupQueue;
    static vector<unsigned> _ordersDeliverySpans;
    static DriverType _pickingUpOrders;
    static unsigned _orderNumberCounter;
    static unsigned _orderQualityCheckDone;
    static double _totalExpenses;
    static double _totalIncomes;
    static void PrintAverage();
    static void PrintDelayed();
    static void NextOrder();

    double _Start = 0;
    queue<Order *> *_DeliveryQueue = nullptr;
    DriverType _MyDriver = Unset;
    unsigned _OrderNumber;
    void DeliveryDelay();
    void PaymentDelay();
    void Behavior();

public:
    static void Stats();
    static void CheckQuality(unsigned orderNumber);

    static unsigned maximumDeliveryTime;
    static unsigned travelTime;
    static unsigned orderPreparation;
    static double averageExpense;
    static double bikesToCars;
    static double averageIncome;
    const unsigned * const OrderNumber = &_OrderNumber;
    void StartImmidiateDelivery();
    void StartDelivery(queue<Order *> *queue);
};


class QualityControl : public Process
{
private:
    unsigned _OrderNumber;

public:
    QualityControl(unsigned orderNumber);
    void Behavior();
};
