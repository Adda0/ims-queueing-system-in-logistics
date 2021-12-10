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
        cout << "Invalid arguments." << endl;
        HelpMsg();
        exit(0);
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
    // accepted long options
    static struct option longOptions[] =
    {
        {"help",          0, nullptr, 'h'},
        {"expenses",      0, nullptr, 'e'},
        {"incomes",       0, nullptr, 'i'},
        {"cars",          0, nullptr, 'c'},
        {"bikes",         0, nullptr, 'b'},
        {"order",         0, nullptr, 'o'},
        {"rush",          0, nullptr, 'r'},
        {"preparation",   0, nullptr, 'p'},
        {"delivery",      0, nullptr, 'd'},
        {"travel",        0, nullptr, 't'},
        {"quality",       0, nullptr, 'q'},
        {"unpaid",        0, nullptr, 'u'},
        {nullptr,         0, nullptr,  0 }
    };

    int c{-1};
    unsigned long converter{0};

    opterr = 0;
    while ((c = getopt_long(argc, argv, "?he:i:c:b:o:r:p:d:t:q:u:", longOptions, nullptr)) != -1)
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
                Order::unpaidProbability = stod(optarg);
                if (Order::unpaidProbability < 0.0 || Order::unpaidProbability > 1.0)
                {
                    throw exception();
                }
                break;

            case 'h':
            case '?':
                HelpMsg();
                exit(0);
                break;

            default:
                throw exception();
                break;
        }    
    }

    if (_cars == 0 && _bikes == 0)
    {
        throw exception();
    }

    Order::bikesToCars = static_cast<double>(_bikes) / (_bikes + _cars);
}

void Program::HelpMsg()
{
    cout << "Here will be some nicely fomated help message" << endl;
}
