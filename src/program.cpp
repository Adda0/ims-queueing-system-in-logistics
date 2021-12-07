#include "program.h"

unsigned Program::_cars(4);
unsigned Program::_bikes(6);
unsigned Program::_deliveryTime(60);

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
    Order::Stats(_deliveryTime);
}

void Program::ParseArguments(int argc, char **argv)
{
    unsigned orderSpan{5};
    unsigned orderSpanRushHour{0};
    unsigned orderPreparation{10};
    unsigned travel{12};
    unsigned travelMin{0};
    unsigned travelMax{0};

    // accepted long options
    static struct option longOptions[] =
    {
        {"bikes",         0, nullptr, 'b'},
        {"cars",          0, nullptr, 'c'},
        {"order",         0, nullptr, 'o'},
        {"rush",          0, nullptr, 'r'},
        {"preparation",   0, nullptr, 'p'},
        {"help",          0, nullptr, 'h'},
        {"delivery",      0, nullptr, 'd'},
        {"travel",        0, nullptr, 't'},
        {"travel-min",    0, nullptr, 'n'},
        {"travel-max",    0, nullptr, 'x'},
        {nullptr,         0, nullptr,  0 }
    };

    int c{-1};
    unsigned long converter{0};

    opterr = 0;
    while ((c = getopt_long(argc, argv, "?hb:c:o:d:p:t:n:x:", longOptions, nullptr)) != -1)
    {
        switch (c)
        {
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
                orderSpan = converter;
                break;
            
            case 'd':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                _deliveryTime = converter;
                break;

            case 'p':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                orderPreparation = converter;
                break;
            
            case 'r':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                orderSpanRushHour = converter;
                break;
            
            case 't':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                travel = converter;
                break;
            
            case 'n':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                travelMin = converter;
                break;
            
            case 'x':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                travelMax = converter;
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

    Order::SetTravelTimes(travel, travelMin, travelMax);
    Order::SetPreparationTime(orderPreparation);
    Order::SetBikesToCars(_bikes, _cars);
    Generator::SetOrderSpans(orderSpan, orderSpanRushHour);
}

void Program::HelpMsg()
{
    cout << "Here will be some nicely fomated help message" << endl;
}
