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
    Packing
};

#define CAR_DRIVER_ORDER_COUNT 4
#define CASH_PAYMENT_PROBABILITY 0.1
#define CASH_PAYMENT_DELAY 2
#define CARD_PAYMENT_PROBABILITY 0.2
#define CARD_PAYMENT_DELAY 1
#define PAIED_IN_ADVANCED_PROBABILITY (1.0 - CASH_PAYMENT_PROBABILITY - CARD_PAYMENT_PROBABILITY)
#define TRAFIC_JAM_PROBABILITY 0.5
#define EXPIRED_DELIVERY_PAID_PROBABILITY 0.5
#define DELIVERY_DELAY_MIN 1
#define DELIVERY_DELAY_MAX 5
#define PACKING_DELAY_MIN 1
#define PACKING_DELAY_MAX 4
#define DRIVER_MISTAKE_PROBABILITY 0.01
#define DRIVER_MISTAKE_TIME 10

#define QUALITY_DELAY_TIME_DEFAULT 20
#define TRAVEL_TIME_DEFAULT 12
#define PREPARATION_TIME_DEFAULT 10
#define MAXIMUM_DELIVERY_TIME_DEFAULT 60
#define AVERAGE_EXPENSES_DEFAULT 100
#define AVERAGE_INCOMES_DEFAULT 250

class Order : public Process
{
private:
    static Facility _driverPreparationFacility;
    static Queue _waitingOrdersQueue;
    static Queue _driverPackingQueue;
    static Histogram _deliveryTimeHistogram;
    static Histogram _driverLoadHistogram;
    static queue<Order *> *_preparedForPickupQueue;
    static vector<unsigned> _deliverySpans;
    static DriverType _pickingUpOrders;
    static unsigned _orderNumberCounter;
    static double _totalExpenses;
    static double _totalIncomes;
    static unsigned _orderQualityCheckDone;
    static void PrintAverage();
    static void PrintDelayed();
    static void PrintRemade();
    static void PrintEconomics();
    static void NextOrder();

    double _Start = 0;
    queue<Order *> *_DeliveryQueue = nullptr;
    DriverType _MyDriver = Unset;
    unsigned _OrderNumber;
    void DeliveryDelay();
    void HandoutDelay();
    void PaymentDelay();
    void Behavior();
    void NextPacking();

public:
    static void Stats();
    static void CheckQuality(unsigned orderNumber);

    static unsigned maximumDeliveryTime;
    static unsigned travelTime;
    static unsigned preparationTime;
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
    static unsigned qualityDelay;

    QualityControl(unsigned orderNumber);
    void Behavior();
};
