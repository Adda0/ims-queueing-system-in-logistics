//===============================================================================================================
// File:        program.cpp
// Project:     VUT, FIT, IMS, Queueing system in logistics
// Date:        11. 12. 2021
// Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
//===============================================================================================================

#include "program.h"

unsigned Program::_cars(NUMBER_OF_CARS_DEFAULT);
unsigned Program::_bikes(NUMBER_OF_BIKES_DEFAULT);

void Program::Run(int argc, char **argv)
{
    try
    {
        ParseArguments(argc, argv);
    }
    catch (exception &e)
    {
        (void)e;
        cerr << "Invalid arguments." << endl;
        HelpMessage();
        exit(1);
    }

    Init(0, OPENING_HOURS + ADDITIONAL_DELIVERY_TIME);
    Cars::Init(_cars);
    Bikes::Init(_bikes);

    DriverLoad driverLoad;
    driverLoad.Activate();
    Generator generator;
    generator.Activate();
    simlib3::Run();

    if (DriverLoad::printCSV)
    {
        driverLoad.PrintCSV();
    }
    else
    {
        Cars::Stats();
        Bikes::Stats();
        Order::Stats();
    }
}

void Program::ParseArguments(int argc, char **argv)
{
    // accepted options
    static struct option longOptions[] =
    {
        {"help",          0, nullptr, 'h'},
        {"expense",       1, nullptr, 'e'},
        {"income",        1, nullptr, 'i'},
        {"cars",          1, nullptr, 'c'},
        {"bikes",         1, nullptr, 'b'},
        {"order",         1, nullptr, 'o'},
        {"rush",          1, nullptr, 'r'},
        {"preparation",   1, nullptr, 'p'},
        {"delivery",      1, nullptr, 'd'},
        {"travel",        1, nullptr, 't'},
        {"quality",       1, nullptr, 'q'},
        {"unpaid",        1, nullptr, 'u'},
        {"file",          0, nullptr, 'f'},
        {nullptr,         0, nullptr,  0 }
    };

    int c{-1};
    unsigned long converter{0};

    opterr = 0;
    while ((c = getopt_long(argc, argv, "he:i:c:b:o:r:p:d:t:q:u:f", longOptions, nullptr)) != -1)
    {
        switch (c)
        {
            case 'e':
                Order::averageExpense = stod(optarg);
                if (Order::averageExpense <= 0.0)
                {
                    throw exception();
                }
                break;
            
            case 'i':
                Order::averageIncome = stod(optarg);
                if (Order::averageIncome <= 0.0)
                {
                    throw exception();
                }
                break;

            case 'b':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                _bikes = converter;
                break;
            
            case 'c':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                _cars = converter;
                break;
            
            case 'o':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                Generator::orderSpan = converter;
                break;
            
            case 'r':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                Generator::orderSpanRushHour = converter;
                break;

            case 'p':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                Order::preparationTime = converter;
                break;

            case 'd':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                Order::maximumDeliveryTime = converter;
                break;
            
            case 't':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                Order::travelTime = converter;
                break;

            case 'q':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                QualityControl::qualityDelay = converter;
                break;

            case 'u':
                Order::unpaidProbability = stod(optarg) / 100;
                if (Order::unpaidProbability < 0.0 || Order::unpaidProbability > 1.0)
                {
                    throw exception();
                }
                break;
            
            case 'f':
                DriverLoad::printCSV = true;
                break;

            case 'h':
                HelpMessage();
                exit(0);
                break;

            case '?':
            default:
                throw exception();
                break;
        }    
    }

    if (_cars == 0 && _bikes == 0)  // delivery cannot be done without vehicles
    {
        throw exception();
    }

    Order::bikesToCars = static_cast<double>(_bikes) / (_bikes + _cars);
}

void Program::HelpMessage()
{
    cout << "Usage: model [OPTIONS]" << endl;
    cout << "Options:" << endl;
    cout << "  -e or --expense NUM      Sets the average expense for one order, default value is " << Order::averageExpense << "." << endl;
    cout << "  -i or --income NUM       Sets the average income for one order, default value is " << Order::averageIncome << "." << endl;
    cout << "  -c or --cars NUM         Sets the number of used cars, default value is " << Program::_cars << "." << endl;
    cout << "  -b or --bikes NUM        Sets the number of used motorbikes, default value is " << Program::_bikes << "." << endl;
    cout << "  -o or --order NUM        Sets the average time span in minutes between orders, default value is " << Generator::orderSpan << "." << endl;
    cout << "  -r or --rush NUM         Sets the average time span in minutes between orders during the lunch time, default value is " << Generator::orderSpanRushHour << "." << endl;
    cout << "  -p or --preparation NUM  Sets the average time in minutes needed for meal preparation, default value is " << Order::preparationTime << "." << endl;
    cout << "  -d or --delivery NUM     Sets the maximum time in minutes, in wich an order has to be delivered, default value is " << Order::maximumDeliveryTime << "." << endl;
    cout << "  -t or --travel NUM       Sets the average time in minutes, which a driver spends traveling between orders or restaurant, default value is " << Order::travelTime << "." << endl;
    cout << "  -q or --quality NUM      Sets the time in minutes, after which an order delivery must start, default value is " << QualityControl::qualityDelay << ". In case no drivers are available order must be prepared again." << endl;
    cout << "  -u or --unpaid NUM       Sets the probability in percentages of delayed order not being paid, default value is " << Order::unpaidProbability << "." << endl;
    cout << "  -f or --file             Prints the load of drivers during each minute in CSV format instead of the regular output." << endl;
    cout << "  -h or --help             Prints this help message." << endl;
}
