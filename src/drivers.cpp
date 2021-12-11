//===============================================================================================================
// File:        drivers.cpp
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#include "drivers.h"

const Store * const Cars::cars = &Cars::_store;
const Store * const Bikes::bikes = &Bikes::_store;

bool DriverLoad::printCSV(false);

void DriverLoad::Behavior()
{
    _driverLoad.push_back( 
    { 
        .time = static_cast<unsigned>(Time), 
        .carLoad = static_cast<unsigned>(Cars::cars->Used()), 
        .bikeLoad = static_cast<unsigned>(Bikes::bikes->Used()) 
    });

    Activate(Time + 1);
}

void DriverLoad::PrintCSV()
{
    cout << "time,cars,bikes" << endl;
    for (auto line : _driverLoad)
    {
        cout << line.time << "," << line.carLoad << "," << line.bikeLoad << endl;
    }
}

