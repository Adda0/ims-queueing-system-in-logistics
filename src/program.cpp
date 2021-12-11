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

    Generator generator;
    generator.Activate();
    simlib3::Run();

    Cars::Stats();
    Bikes::Stats();
    Order::Stats();
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
        {nullptr,         0, nullptr,  0 }
    };

    int c{-1};
    unsigned long converter{0};

    opterr = 0;
    while ((c = getopt_long(argc, argv, "he:i:c:b:o:r:p:d:t:q:u:", longOptions, nullptr)) != -1)
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
                Generator::orderSpanRush = converter;
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
    cout << "  -h or --help             prints this help message." << endl;
    cout << "  -e or --expense NUM      sets the average expense for one order, default value is " << Order::averageExpense << "." << endl;
    cout << "  -i or --income NUM       sets the average income for one order, default value is " << Order::averageIncome << "." << endl;
    cout << "  -c or --cars NUM         sets the number of used cars, default value is " << Program::_cars << "." << endl;
    cout << "  -b or --bikes NUM        sets the number of used motorbikes, default value is " << Program::_bikes << "." << endl;
    cout << "  -o or --order NUM        sets the average time span in minutes between orders, default value is " << Generator::orderSpan << "." << endl;
    cout << "  -r or --rush NUM         sets the average time span in minutes between orders during the lunch time, default value is " << Generator::orderSpanRush << "." << endl;
    cout << "  -p or --preparation NUM  sets the average time in minutes needed for meal preparation, default value is " << Order::preparationTime << "." << endl;
    cout << "  -d or --delivery NUM     sets the maximum time in minutes, in wich an order has to be delivered, default value is " << Order::maximumDeliveryTime << "." << endl;
    cout << "  -t or --travel NUM       sets the average time in minutes, which a driver spends traveling between orders or restaurant, default value is " << Order::travelTime << "." << endl;
    cout << "  -q or --quality NUM      sets the time in minutes, after which an order delivery must start, default value is " << QualityControl::qualityDelay << ". In case no drivers are available order must be prepared again." << endl;
    cout << "  -u or --unpaid NUM       sets the probability in percentages of delayed order not being paid, default value is " << Order::unpaidProbability << "." << endl;
}
