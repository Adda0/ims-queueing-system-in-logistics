#include "drivers.h"

Store Drivers::_store;
Queue Drivers::_queue;

Store *Drivers::get_drivers()
{
    return &_store;
}

void Drivers::init_drivers(unsigned long capacity)
{
    _store.SetQueue(&_queue);
    _store.SetCapacity(capacity);
}
