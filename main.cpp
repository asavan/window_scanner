#include "Scanner/storage.h"

#include "BMPGenerator/window2bmp.h"
#include "BMPGenerator/config.h"
#include "BMPGenerator/BmpAdaptor.h"
#include <iostream>

#include "Scanner/image.h"
#include "Utils/sleeper.h"

#include <memory>

using namespace std::chrono_literals;


static std::shared_ptr<Storage> makeStorage(int argc, char* argv[])
{
    std::shared_ptr<Storage> storage;
    if (argc < 3)
    {
        std::cout << "Default arguments" << std::endl;
        storage = std::make_shared<Storage>("../base/1680x1050/layout", "../base/1680x1050", true);
    }
    else
    {
        storage = std::make_shared<Storage>(argv[1], argv[2]);
    }

    if (storage->hasUnlearnedCards())
    {
        std::cout << "Study not complete " << std::endl;
        exit(1);
    }
    std::cout << "Study complete " << std::endl;
    return storage;
}

bool recognizeOneImage(const Storage& storage, int& errCount, unsigned long long hwndInt, int count)
{
    BmpAdaptor adaptor;
    if (adaptor.captureWindow(hwndInt))
    {
        errCount = 0;
        std::string str = storage.getStringFromImage(adaptor.getImage());
        if ((count % 100) == 0)
        {
            std::cout << str << std::endl;
        }
        return true;
    }
    return false;
}


int main(int argc, char* argv[])
{
    try {
        std::shared_ptr<Storage> storage = makeStorage(argc, argv);
        storage->setLayout("layout.txt");
        int errCount = 0;
        ConfigParser config("config.txt");
        unsigned long long hwndInt = config.processId();
        {
            Sleeper s(0s); // only show time
            for (int i = 0; i < 1000; ++i)
            {
                if (!recognizeOneImage(*storage, errCount, hwndInt, i))
                {
                    ++errCount;
                    if (errCount > 4)
                    {
                        break;
                    }
                    Sleeper s1(5s);
                }
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}
