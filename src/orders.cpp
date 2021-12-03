#include "orders.h"

Queue Order::_driversQueue("Orders");
queue<Order *> *Order::_kitchenQueue(nullptr);
vector<unsigned> Order::_ordersDeliverySpans;
DriverType Order::_driver(Unset);
unsigned Order::_travelMin(0);
unsigned Order::_travelMax(0);
double Order::_bikesToCars(0.5);

void Order::StartDelivery(queue<Order *> *queue)
{
    _deliveryQueue = queue;
    Activate();
};

void Order::Behavior()
{
    _start = Time;
    while (Cars::cars->Full() && Bikes::bikes->Full())
    {
        Into(_driversQueue);
        Passivate();
    }

    if (_driver == BikeRider)
    {
        _myDriver = BikeRider;
        _driver = Unset;
        _kitchenQueue->push(this);
        _kitchenQueue->front()->StartDelivery(_kitchenQueue);
        _kitchenQueue->pop();
        Passivate();
    }
    else if (_driver == CarDriver)
    {
        _myDriver = CarDriver;
        _kitchenQueue->push(this);
        if (_kitchenQueue->size() == 3)
        {
            _driver = Unset;
            _kitchenQueue->front()->StartDelivery(_kitchenQueue);
            _kitchenQueue->pop();
        }
        Passivate();
    }
    else
    {
        _kitchenQueue = new queue<Order *>;
        if ((!Cars::cars->Full() && !Bikes::bikes->Full() && Random() > _bikesToCars) || (!Cars::cars->Full() && Bikes::bikes->Full()))
        {
            Cars::Take(*this);
            _kitchenQueue->push(this);
            _driver = CarDriver;
            _myDriver = CarDriver;
            Passivate();
        }
        else
        {
            Bikes::Take(*this);
            _kitchenQueue->push(this);
            _driver = BikeRider;
            _myDriver = BikeRider;
            Passivate();
        }
    }

    Wait(Uniform(_travelMin, _travelMax));
    _ordersDeliverySpans.push_back(static_cast<unsigned>(Time - _start));

    if (_deliveryQueue->empty())
    {
        if (_myDriver == CarDriver)
        {
            Cars::Deliver(*this);
        }
        else
        {
            Bikes::Deliver(*this);
        }

        delete _deliveryQueue;

        if (!_driversQueue.Empty())
        {
            _driversQueue.GetFirst()->Activate();
        }
    }
    else
    {
        _deliveryQueue->front()->StartDelivery(_deliveryQueue);
        _deliveryQueue->pop();
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
    cout << "Average order delivery time: " << average / 60 << " minutes and " << average % 60 << " seconds." << endl;
}

void Order::PrintDelayed(unsigned cutoff)
{
    unsigned delayed = 0;
    for (auto order : _ordersDeliverySpans)
    {
        if (order > cutoff)
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
        cout << "There was 1 delayed order." << endl;
    }
    else
    {
        cout << "There were " << delayed << " delayed orders" << endl;
    }
}

void Order::SetTravelTimes(unsigned min, unsigned max)
{
    _travelMin = min;
    _travelMax = max;
}

void Order::SetBikesToCars(unsigned bikes, unsigned cars)
{
    if (bikes || cars)
    {
        _bikesToCars = static_cast<double>(bikes) / (bikes + cars);
    }
    else
    {
        throw exception();
    }
}

void Order::Stats(unsigned cuttoff)
{
    _driversQueue.Output();
    PrintAverage();
    PrintDelayed(cuttoff);
}
