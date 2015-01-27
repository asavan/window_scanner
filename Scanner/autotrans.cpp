#include "storage.h"

int main(int argc, char* argv[])
{
    Storage* storage = NULL;
    if (argc < 3)
    {
        std::cout << "Default arguments are used" << std::endl;
        storage = new Storage("1680x1050/layout", "1680x1050");
    }
    else
        storage = new Storage(argv[1], argv[2]);

//    storage->test__("tempbase/1680x1050/layout", "tempbase/1680x1050");
    storage->test__("1680x1050/layout", "1680x1050");

    return 0;
}
