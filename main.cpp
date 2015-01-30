#include "deck.h"
#include "Scanner/storage.h"

#include "BMPGenerator/window2bmp.h"
#include <iostream>

#include "Scanner/image.h"

	

int main(int argc, char* argv[])
{
    Storage* storage = NULL;
    if (argc < 3)
    {
        std::cout << "Default arguments" << std::endl;
        storage = new Storage("../base/1680x1050/layout", "../base/1680x1050", true);
    }
    else
	{
        storage = new Storage(argv[1], argv[2]);
	}
	
	if (storage->hasUnlearnedCards())
	{
		std::cout << "Study not complete " << std::endl;
		return 1;
	}
	std::cout << "Study complete " << std::endl;
	storage->setLayout("layout.txt");
	int errCount = 0;
	for (int i = 0; i < 2000; ++i)
	{
		int hwndInt = getProcessId();
		BmpAdaptor adaptor;
		SaveWindow2BMPRaw(HWND(hwndInt), adaptor);

		if(adaptor.isValid())
		{
			errCount = 0;
			std::string str = storage->getStringFromImage(Image::getImageFromBmp(adaptor.getBmp()));
			std::cout << str << std::endl;
		}
		else 
		{
			++errCount;
			if(errCount > 4) 
			{
				break;
			}
			Sleep(2000);
		}
	}

	delete storage;
    return 0;
}

