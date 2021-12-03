#include "program.h"

unsigned Program::_cars(3);
unsigned Program::_bikes(5);
unsigned Program::_orderSpan(300);
unsigned Program::_orderSpanPeak(0);
unsigned Program::_deliveryTime(3600);
unsigned Program::_travel(900);
unsigned Program::_travelMin(0);
unsigned Program::_travelMax(0);

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
    // accepted long options
    static struct option longOptions[] =
    {
        {"bikes",         0, nullptr, 'b'},
        {"cars",          0, nullptr, 'c'},
        {"order-span",    0, nullptr, 'o'},
        {"peak-span",     0, nullptr, 'p'},
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
                _orderSpan = converter;
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
                _orderSpanPeak = converter;
                break;
            
            case 't':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                _travel = converter;
                break;
            
            case 'n':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                _travelMin = converter;
                break;
            
            case 'x':
                converter = stoul(optarg);
                if (converter > UINT32_MAX)
                {
                    throw exception();
                }
                _travelMax = converter;
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

    if (_orderSpanPeak == 0)
    {
        _orderSpanPeak = _orderSpan >> 1;
    }

    if (_travelMin == 0 || _travelMax == 0)
    {
        _travelMin = _travel > 300 ? _travel - 300 : 0;
        _travelMax = _travel + 300;

        Order::SetTravelTimes(_travelMin, _travelMax);
    }

    Order::SetBikesToCars(_bikes, _cars);
}

void Program::HelpMsg()
{
    cout << "Here will be some nicely fomated help message" << endl;
}
