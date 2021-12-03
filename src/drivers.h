#pragma once

#include <simlib.h>
#include <string>

using namespace std;

template <typename T> class Drivers
{
protected:
    static Store _store;
    
public:
    static void Init(unsigned long capacity);
    static void Enqueue(Process &process);
    static void Take(Process &process);
    static void Deliver(Process &process);
    static void TakeNext();
    static void Stats();  
};

class Cars : public Drivers<Cars>
{
private:

public:
    static const Store * const cars;
};

class Bikes : public Drivers<Bikes>
{
private:

public:
    static const Store * const bikes;
};

template <typename T> Store Drivers<T>::_store;

template <typename T> void Drivers<T>::Init(unsigned long capacity)
{
    _store.SetCapacity(capacity);
}

template <typename T> void Drivers<T>::Take(Process &process)
{
    process.Enter(_store);
}

template <typename T> void Drivers<T>::Deliver(Process &process)
{
    process.Leave(_store);
}

template <typename T> void Drivers<T>::Stats()
{
    string name = &typeid(T).name()[1];

    _store.SetName(name);
    _store.Output();
}
