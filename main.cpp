#include "deck.h"
#include "Scanner/storage.h"
#include "BMPGenerator/window2bmp.h"
#include <iostream>

static int getProcessId()
{
	FILE* f;
	f = fopen("config.txt", "r");
	if (!f) throw std::runtime_error("No config file");
	char name[256];
	fscanf(f, "%s", name);
	std::string filename = name;
	// ifs >> filename;
	int hwndInt;
	// ifs >> hwndInt;
	fscanf(f, "%x", &hwndInt);
	printf("%x\n", hwndInt);
	fclose(f);
	return hwndInt;
}
	

int main(int argc, char* argv[])
{
    Storage* storage = NULL;
    if (argc < 3)
    {
        std::cout << "Default arguments are used" << std::endl;
        storage = new Storage("../base/1680x1050/layout", "../base/1680x1050");
    }
    else
	{
        storage = new Storage(argv[1], argv[2]);
	}

	int hwndInt = getProcessId();
	int errCount = 0;
	storage->setLayout("layout.txt");
	for (int i = 0; i < 2000; ++i)
	{
		BmpAdaptor adaptor;
		SaveWindow2BMPRaw(HWND(hwndInt), adaptor);
		if(adaptor.isValid())
		{
			std::string str = storage->getStringFromBmp(adaptor.getBmp());
			std::cout << str << std::endl;
		}
		else 
		{
			errCount++;
			if(errCount > 4) 
			{
				break;
			}
		// Sleep(100);
		}
	}

	delete storage;
    return 0;
}

