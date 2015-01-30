#include "deck.h"
#include "Scanner/storage.h"

#include "BMPGenerator/window2bmp.h"
#include <iostream>

#include "Scanner/image.h"

#include <boost/smart_ptr/shared_ptr.hpp>


static boost::shared_ptr<Storage> makeStorage(int argc, char* argv[])
{
	boost::shared_ptr<Storage> storage;
    if (argc < 3)
    {
        std::cout << "Default arguments" << std::endl;
		storage = boost::shared_ptr<Storage>(new Storage("../base/1680x1050/layout", "../base/1680x1050", true));
    }
    else
	{
        storage = boost::shared_ptr<Storage>(new Storage(argv[1], argv[2]));
	}
	
	if (storage->hasUnlearnedCards())
	{
		std::cout << "Study not complete " << std::endl;
		exit(1);
	}
	std::cout << "Study complete " << std::endl;
	return storage;
}

bool recognizeOneImage(const Storage& storage, int& errCount)
{
	int hwndInt = getProcessId();
	BmpAdaptor adaptor;
	if(adaptor.captureWindow(hwndInt))
	{
		errCount = 0;
		Sleeper s(0); // only show time
		std::string str = storage.getStringFromImage(adaptor.getImage());
		std::cout << str << std::endl;
		return true;
	}
	return false;
}
	

int main(int argc, char* argv[])
{
	boost::shared_ptr<Storage> storage = makeStorage(argc, argv);
	storage->setLayout("layout.txt");
	int errCount = 0;
	for (int i = 0; i < 2000; ++i)
	{				
		if(!recognizeOneImage(*storage, errCount))
		{			
			++errCount;
			if(errCount > 4) 
			{
				break;
			}
			Sleeper s(2);
		}
	}
    return 0;
}
