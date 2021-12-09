#include "orders.h"

Queue Order::_waitingOrders("Orders");
queue<Order *> *Order::_preparedForPickupQueue(nullptr);
vector<unsigned> Order::_ordersDeliverySpans;
DriverType Order::_pickingUpOrders(Unset);
double Order::_totalExpenses(0);
unsigned Order::_orderNumberCounter(0);
unsigned Order::_orderQualityCheckDone(0);
double Order::_totalIncomes(0);

unsigned Order::travelTime(12);
unsigned Order::orderPreparation(10);
unsigned Order::maximumDeliveryTime(60);
double Order::bikesToCars(0.5);
double Order::averageExpense(100);
double Order::averageIncome(250);

void Order::StartDelivery(queue<Order *> *queue)
{
    _DeliveryQueue = queue;
    Activate();
};

void Order::Behavior()
{
    _Start = Time; // start of order delivery time

    do
    {
        _totalExpenses += averageExpense;
        _OrderNumber = ++_orderNumberCounter;                        // order is assigned with serial number 
        Wait(Normal(orderPreparation, orderPreparation / 4.0));    // cooking and preparation
        (new QualityControl(_OrderNumber))->Activate();              // starting the quality check

        if (Cars::cars->Full() && Bikes::bikes->Full()) 
        {
            // all cars and bikes are currently bussy delivering
            _waitingOrders.InsFirst(this);
            Passivate();
        }

        Priority++;                                 // preemtively increasing the priority of the order, if it fails quality check
    }
    while (_OrderNumber <= _orderQualityCheckDone); // order was not picked up by driver at all yet, quality check failed, must be remade

    if (_pickingUpOrders == BikeRider)              // bike rider is picking 2 orders at once
    {
        _MyDriver = BikeRider;
        _pickingUpOrders = Unset;
        _preparedForPickupQueue->push(this);
        _preparedForPickupQueue->front()->StartDelivery(_preparedForPickupQueue); // delivery of the first order in the batch starts
        _preparedForPickupQueue->pop();
        Passivate();
    }
    else if (_pickingUpOrders == CarDriver) // car driver is picking 4 orders at once
    {
        _MyDriver = CarDriver;
        _preparedForPickupQueue->push(this);
        if (_preparedForPickupQueue->size() == CAR_DRIVER_ORDER_COUNT)
        {
            _pickingUpOrders = Unset;
            _preparedForPickupQueue->front()->StartDelivery(_preparedForPickupQueue); // delivery of the first order in the batch starts
            _preparedForPickupQueue->pop();
        }
        else
        {
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
            _pickingUpOrders = CarDriver;   // current driver, who is picking deliveries
            _MyDriver = CarDriver;          // driver of the given order
        }
        else
        {
            Bikes::Take(*this);             // takes the current bike
            _preparedForPickupQueue->push(this);
            _pickingUpOrders = BikeRider;   // current driver, who is picking deliveries
            _MyDriver = BikeRider;          // driver of the given order
        }
        
        Wait(Uniform(5, 10));               // TODO
        NextOrder();                        // driver can take more orders
        Passivate();
    }

    DeliveryDelay(); // driver drives to the customer
    PaymentDelay();  // driver hands the order to customer and recieves payment
    unsigned travelTime{static_cast<unsigned>(Time - _Start)};
    _ordersDeliverySpans.push_back(travelTime);   // order delivered
    if (travelTime > maximumDeliveryTime)
    {
        if (Random() < EXPIRED_DELIVERY_PAID_PROBABILITY)
        {
            _totalIncomes += averageIncome / 2.0;  // order deliverted in time, but the customer accepts the order and pays half the price
        }
    }
    else
    {
        _totalIncomes += averageIncome;     // order deliver in time, therfore paid 
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
        _DeliveryQueue->front()->StartDelivery(_DeliveryQueue); // delivery of another order in a batch is started
        _DeliveryQueue->pop();
    }
}

void Order::PrintAverage()
{
    unsigned long sum = 0;
    for (auto order : _ordersDeliverySpans)
    {
        sum += order;
    }
    unsigned average = sum / _ordersDeliverySpans.size();
    cout << "Average order delivery time was " << average << " minutes." << endl;
}

void Order::PrintDelayed()
{
    unsigned delayed = 0;
    for (auto order : _ordersDeliverySpans)
    {
        if (order > maximumDeliveryTime)
        {
            delayed += 1;
        }
    }

    if (delayed == 0)
    {
        cout << "There were not any delayed orders." << endl;
    }
    else if (delayed == 1)
    {
        cout << "There was 1 delayed order " << _ordersDeliverySpans.size() << " served." << endl;
    }
    else
    {
        cout << "There were " << delayed << " delayed orders of " << _ordersDeliverySpans.size() << " served." << endl;
    }
}

void Order::Stats()
{
    _waitingOrders.Output();
    PrintAverage();
    PrintDelayed();
}

void Order::NextOrder()
{
    if (!_waitingOrders.Empty())
    {
        _waitingOrders.GetFirst()->Activate();  // new order can be handed to the currently filled vehicle
    }
}

void Order::DeliveryDelay()
{
    // TODO driver mistake

    if (_MyDriver == CarDriver && Generator::IsIncreasedTrafic(Time) && Random() < TRAFIC_JAM_PROBABILITY)
    {
        // Orders is delivered in a car and hits traffic jam, the delivery time is doubled.
        Wait(Normal(travelTime, travelTime / 4.0));
    }
    else
    {
        // normal delivery time
        Wait(Normal(travelTime, travelTime / 4.0));
    }

    // TODO handout
}

void Order::PaymentDelay()
{
    double rand = Random();

    if (rand <= CASH_PAYMENT_PROBABILITY)
    {
        Wait(CASH_PAYMENT_DELAY);
    }
    else if (rand <= PAIED_IN_ADVANCED_PROBABILITY)
    {
        Wait(CARD_PAYMENT_DELAY);
    }
}

void Order::CheckQuality(unsigned orderNumber)
{
    // some driver is collecting orders and the first order is waiting for too long (second, third, ...) orders cannot 
    // wait longer than the first one.
    if (_pickingUpOrders != Unset && *_preparedForPickupQueue->front()->OrderNumber == orderNumber)
    {
        _preparedForPickupQueue->front()->StartImmidiateDelivery();     // driver leaves without filling its full capacity
    }

    _orderQualityCheckDone = orderNumber;   // orders up to this order number were checked for quality
}

void Order::StartImmidiateDelivery()
{
    _preparedForPickupQueue->front()->StartDelivery(_preparedForPickupQueue); // delivery of the first order in the batch starts
    _preparedForPickupQueue->pop();
    _pickingUpOrders = Unset;   // driver leaves  
}


QualityControl::QualityControl(unsigned orderNumber) : _OrderNumber(orderNumber) {}

void QualityControl::Behavior()
{
    Wait(QUALITY_DELAY_TIME);
    Order::CheckQuality(_OrderNumber);
}

