//===============================================================================================================
// File:        orders.h
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

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
#define CASH_PAYMENT_PROBABILITY 0.05
#define CASH_PAYMENT_DELAY 2
#define CARD_PAYMENT_PROBABILITY 0.05
#define CARD_PAYMENT_DELAY 1
#define PAID_IN_ADVANCED_PROBABILITY (1.0 - CASH_PAYMENT_PROBABILITY - CARD_PAYMENT_PROBABILITY)
#define TRAFFIC_JAM_PROBABILITY 40
#define DELIVERY_DELAY_MIN 1
#define DELIVERY_DELAY_MAX 4
#define PACKING_DELAY_MIN 1
#define PACKING_DELAY_MAX 5
#define DRIVER_MISTAKE_PROBABILITY 0.01
#define COOK_MISTAKE_PROBABILITY 0.01
#define DRIVER_MISTAKE_TIME 10

#define QUALITY_DELAY_TIME_DEFAULT 20
#define TRAVEL_TIME_DEFAULT 12
#define PREPARATION_TIME_DEFAULT 7
#define MAXIMUM_DELIVERY_TIME_DEFAULT 60
#define AVERAGE_EXPENSES_DEFAULT 46
#define AVERAGE_INCOMES_DEFAULT 325
#define UNPAID_ORDER_PROBABILITY_DEFAULT 0.5

class Order : public Process
{
private:
    static Facility _driverPreparationFacility;
    static Queue _waitingOrdersQueue;
    static Queue _driverPackingQueue;
    static Histogram _deliveryTimeHistogram;
    static Histogram _carDeliveryTimeHistogram;
    static Histogram _bikeDeliveryTimeHistogram;
    static Histogram _driverLoadHistogram;
    static queue<Order *> *_preparedForPickupQueue;
    static vector<unsigned> _deliverySpans;
    static DriverType _pickingUpOrders;
    static unsigned _orderNumberCounter;
    static double _totalExpenses;
    static double _totalIncomes;
    static unsigned _orderQualityCheckDone;
    static unsigned _cookMistakes;
    static unsigned _driverMistakes;
    static void PrintAverage();
    static void PrintDelayed();
    static void PrintRemade();
    static void PrintEconomics();
    static void PrintMistakes();
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
    static double unpaidProbability;
    const unsigned * const OrderNumber = &_OrderNumber;
    void StartImmediateDelivery();
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
