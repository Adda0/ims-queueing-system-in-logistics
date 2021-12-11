//===============================================================================================================
// File:        orders.cpp
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#include "orders.h"

Queue Order::_waitingOrdersQueue("Orders waiting for a driver.");
Queue Order::_driverPackingQueue("Orders waiting to be packed by a driver.");
Histogram Order::_deliveryTimeHistogram("Order delivery time.", 10, 10, 20);
Histogram Order::_driverLoadHistogram("Orders delivered by a driver at once.", 1, 1, 4);
Facility Order::_driverPreparationFacility;
queue<Order *> *Order::_preparedForPickupQueue(nullptr);
vector<unsigned> Order::_deliverySpans;
DriverType Order::_pickingUpOrders(Unset);
double Order::_totalExpenses(0);
unsigned Order::_orderNumberCounter(0);
unsigned Order::_orderQualityCheckDone(0);
double Order::_totalIncomes(0);
double Order::bikesToCars(0);
unsigned Order::_cookMistakes(0);
unsigned Order::_driverMistakes(0);

unsigned Order::travelTime(TRAVEL_TIME_DEFAULT);
unsigned Order::preparationTime(PREPARATION_TIME_DEFAULT);
unsigned Order::maximumDeliveryTime(MAXIMUM_DELIVERY_TIME_DEFAULT);
double Order::averageExpense(AVERAGE_EXPENSES_DEFAULT);
double Order::averageIncome(AVERAGE_INCOMES_DEFAULT);
double Order::unpaidProbability(UNPAID_ORDER_PROBABILITY_DEFAULT);

unsigned QualityControl::qualityDelay(QUALITY_DELAY_TIME_DEFAULT);

void Order::StartDelivery(queue<Order *> *queue)
{
    _DeliveryQueue = queue;
    _driverLoadHistogram(queue->size() + 0.000001);
    Activate();
};

void Order::Behavior()
{
    _Start = Time; // start of order delivery time

    do
    {
        _OrderNumber = ++_orderNumberCounter;                        // order is assigned with serial number

        _totalExpenses += averageExpense;
        Wait(Lim(Normal(preparationTime, preparationTime / 4.0), 0, preparationTime * 2).Value());        // cooking and preparation

        if (Priority == 0 && Random() < COOK_MISTAKE_PROBABILITY)    // Cook makes a mistake and the meal must be prepared again
        {
            _cookMistakes++;
            _totalExpenses += averageExpense;
            Wait(Lim(Normal(preparationTime, preparationTime / 4.0), 0, preparationTime * 2).Value());
        }
 
        (new QualityControl(_OrderNumber))->Activate();              // starting the quality check

        if (Cars::cars->Full() && Bikes::bikes->Full())
        {
            // all cars and bikes are currently bussy delivering
            _waitingOrdersQueue.InsLast(this);
            Passivate();
        }

        if (_driverPreparationFacility.Busy())
        {
            _driverPackingQueue.InsLast(this);      // driver is currently packing
            Passivate();
        }
        Priority++;                                 // preemtively increasing the priority of the order, if it fails quality check
    }
    while (_OrderNumber <= _orderQualityCheckDone); // order was not picked up by driver at all yet, quality check failed, must be remade

    _driverPreparationFacility.Seize(this);     // order is being packed

    if (_pickingUpOrders == BikeRider)              // bike rider is picking 2 orders at once
    {
        _MyDriver = BikeRider;
        _preparedForPickupQueue->push(this);
        _pickingUpOrders = Packing;
        Wait(Uniform(PACKING_DELAY_MIN, PACKING_DELAY_MAX));  // driver packs the food, prepares drinks, ...
        NextPacking();      // driver can pack next order
        _pickingUpOrders = Unset;
        _preparedForPickupQueue->front()->StartDelivery(_preparedForPickupQueue); // delivery of the first order in the batch starts
        _preparedForPickupQueue->pop();
        Passivate();
    }
    else if (_pickingUpOrders == CarDriver) // car driver is picking 4 orders at once
    {
        _MyDriver = CarDriver;
        _preparedForPickupQueue->push(this);
        _pickingUpOrders = Packing;
        Wait(Uniform(PACKING_DELAY_MIN, PACKING_DELAY_MAX));  // driver packs the food, prepares drinks, ...
        NextPacking();      // driver can pack next order
        if (_preparedForPickupQueue->size() == CAR_DRIVER_ORDER_COUNT ||
            _preparedForPickupQueue->front()->_OrderNumber <= _orderQualityCheckDone) // driver capacity is full or the delivery must start imidiately
        {
            _pickingUpOrders = Unset;
            _preparedForPickupQueue->front()->StartDelivery(_preparedForPickupQueue); // delivery of the first order in the batch starts
            _preparedForPickupQueue->pop();
        }
        else
        {
            _pickingUpOrders = CarDriver;
            NextOrder(); // driver can take more orders
        }
        Passivate();
    }
    else    // no driver is picking up orders
    {
        _preparedForPickupQueue = new queue<Order *>;
        // next orders are going to be handled by a type of a driver proportionaly to the available cars and bikes
        if ((!Cars::cars->Full() && !Bikes::bikes->Full() && Random() > bikesToCars) || (!Cars::cars->Full() && Bikes::bikes->Full()))
        {
            Cars::Take(*this);              // takes the current car
            _preparedForPickupQueue->push(this);
            _MyDriver = CarDriver;          // driver of the given order
        }
        else
        {
            Bikes::Take(*this);             // takes the current bike
            _preparedForPickupQueue->push(this);
            _MyDriver = BikeRider;          // driver of the given order
        }

        _pickingUpOrders = Packing;
        Wait(Uniform(PACKING_DELAY_MIN, PACKING_DELAY_MAX));  // driver packs the food, prepares drinks, ...
        NextPacking();      // driver can pack next order
        if (_preparedForPickupQueue->front()->_OrderNumber <= _orderQualityCheckDone) // first order needs to be delivered immidiately
        {
            _pickingUpOrders = Unset;
            _preparedForPickupQueue->pop();
            _driverLoadHistogram(1.00000001);
            _DeliveryQueue = _preparedForPickupQueue;
        }
        else
        {
            _pickingUpOrders = _MyDriver;
            NextOrder();    // driver can take more orders
            Passivate();
        }
    }

    DeliveryDelay(); // driver drives to the customer
    unsigned travelTime{static_cast<unsigned>(Time - _Start)};
    _deliverySpans.push_back(travelTime);   // order delivered
    _deliveryTimeHistogram(travelTime);
    HandoutDelay();  // driver waits for the customer to pick up the food
    PaymentDelay();  // driver hands the order to customer and recieves payment

    if (travelTime <= maximumDeliveryTime || Random() > unpaidProbability)
    {
        // order delivered in time, therfore paid, or the customer pays delayd order, with certain probability
        _totalIncomes += averageIncome;
    }

    if (_DeliveryQueue->empty()) // no more orders to deliver in a batch
    {
        DeliveryDelay(); // driver drives back to the restaurant

        if (_MyDriver == CarDriver)
        {
            Cars::Deliver(*this);   // car is back from a round of deliveries
        }
        else
        {
            Bikes::Deliver(*this);  // bike is back from a round of deliveries
        }

        delete _DeliveryQueue;
        NextOrder();    // driver arrived back to kitchen and can take next order
    }
    else
    {
        _DeliveryQueue->front()->StartDelivery(_DeliveryQueue); // delivery of another order in the current batch is started
        _DeliveryQueue->pop();
    }
}

void Order::PrintAverage()
{
    unsigned long sum = 0;
    for (auto order : _deliverySpans)
    {
        sum += order;
    }
    unsigned average = sum / _deliverySpans.size();
    cout << "Average order delivery time was " << average << " minutes." << endl;
}

void Order::PrintDelayed()
{
    unsigned delayed{0};
    for (auto order : _deliverySpans)
    {
        if (order > maximumDeliveryTime)
        {
            delayed += 1;
        }
    }

    if (delayed == 0)
    {
        cout << "There were not any delayed orders out of " << _deliverySpans.size() << " served." << endl;
    }
    else if (delayed == 1)
    {
        cout << "There was 1 delayed order out of" << _deliverySpans.size() << " served." << endl;
    }
    else
    {
        cout << "There were " << delayed << " delayed orders out of " << _deliverySpans.size() << " served." << endl;
    }
}

void Order::PrintRemade()
{
    unsigned long remade{_orderNumberCounter - _deliverySpans.size()};
    if (remade == 0)
    {
        cout << "There were not any remade orders." << endl;
    }
    else if (remade == 1)
    {
        cout << "There was 1 remade order." << endl;
    }
    else
    {
        cout << "There were " << remade << " remade orders." << endl;
    }
}

void Order::PrintEconomics()
{
    cout << "Expenses: " << _totalExpenses << endl;
    cout << "Incomes:  " << _totalIncomes << endl;
    cout << "Profit:   " << _totalIncomes - _totalExpenses << endl;
}

void Order::PrintMistakes()
{
    if (_driverMistakes == 0)
    {
        cout << "Drivers did not do any mistakes." << endl;
    }
    else if (_driverMistakes == 1)
    {
        cout << "Drivers made 1 mistake." << endl;
    }
    else
    {
        cout << "Drivers made " << _driverMistakes << " mistakes." << endl;
    }

    if (_cookMistakes == 0)
    {
        cout << "Cooks did not do any mistakes." << endl;
    }
    else if (_cookMistakes == 1)
    {
        cout << "Cooks made 1 mistake." << endl;
    }
    else
    {
        cout << "Cooks made " << _driverMistakes << " mistakes." << endl;
    }
}

void Order::Stats()
{
    _waitingOrdersQueue.Output();
    _driverPackingQueue.Output();
    _deliveryTimeHistogram.Output();
    _driverLoadHistogram.Output();
    PrintMistakes();
    PrintAverage();
    PrintDelayed();
    PrintRemade();
    PrintEconomics();
}

void Order::NextOrder()
{
    if (!_waitingOrdersQueue.Empty())
    {
        _waitingOrdersQueue.GetFirst()->Activate();  // new order can be handed to the currently filled vehicle
    }
}

void Order::NextPacking()
{
    _driverPreparationFacility.Release(this);       // order is packed
    if (!_driverPackingQueue.Empty())
    {
        _driverPackingQueue.GetFirst()->Activate();  // next order in line can be packed
    }
}

void Order::DeliveryDelay()
{
    if (Random() < DRIVER_MISTAKE_PROBABILITY)
    {
        _driverMistakes++;
        Wait(Lim(Exponential(DRIVER_MISTAKE_TIME), 0, DRIVER_MISTAKE_TIME * 4).Value());  // ridic udelal chybu pri dovazce
    }

    if (_MyDriver == CarDriver && Generator::IsIncreasedTraffic(Time) && Random() < TRAFFIC_JAM_PROBABILITY)
    {
        // Orders is delivered in a car and hits traffic jam, the delivery time is doubled.
        Wait(Lim(Normal(travelTime * 1.5, travelTime / 2.0), 0, 4*travelTime).Value());
    }
    else
    {
        // normal delivery time
        Wait(Lim(Normal(travelTime, travelTime / 4.0), 0, travelTime * 4).Value());
    }
}

void Order::HandoutDelay()
{
    Wait(Uniform(DELIVERY_DELAY_MIN, DELIVERY_DELAY_MAX));
}

void Order::PaymentDelay()
{
    double rand = Random();

    if (rand <= CASH_PAYMENT_PROBABILITY)
    {
        Wait(CASH_PAYMENT_DELAY);
    }
    else if (rand <= PAID_IN_ADVANCED_PROBABILITY)
    {
        Wait(CARD_PAYMENT_DELAY);
    }
    // else the order was paid in advance, therfore no delay
}

void Order::CheckQuality(unsigned orderNumber)
{
    // some driver is collecting orders and the first order is waiting for too long (second, third, ...) orders cannot
    // wait longer than the first one.
    if ((_pickingUpOrders == CarDriver || _pickingUpOrders == BikeRider) && *_preparedForPickupQueue->front()->OrderNumber == orderNumber)
    {
        _preparedForPickupQueue->front()->StartImmediateDelivery();     // driver leaves without filling its full capacity
    }

    _orderQualityCheckDone = orderNumber;   // orders up to this order number were checked for quality
}

void Order::StartImmediateDelivery()
{
    _preparedForPickupQueue->front()->StartDelivery(_preparedForPickupQueue); // delivery of the first order in the batch starts
    _preparedForPickupQueue->pop();
    _pickingUpOrders = Unset;   // driver leaves
}


QualityControl::QualityControl(unsigned orderNumber) : _OrderNumber(orderNumber) {}

void QualityControl::Behavior()
{
    Wait(qualityDelay);
    Order::CheckQuality(_OrderNumber);
}

