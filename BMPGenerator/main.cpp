#include "window2bmp.h"
#include <sstream>
#include <fstream>
void f()
{
	HWND find = FindWindow(NULL, NULL);
	SaveWindow2BMP(find, "1111.bmp");
	int i = 0;
	for ( HWND find2 = find; find2; ++i)
	{
		std::stringstream name;
		name << "111";
		name << i;
		name << ".bmp";
		SaveWindow2BMP(find2, name.str());
		find2 = FindWindowEx(NULL, find2, NULL, NULL);
		
	}
}

int make_many_pictures()
{
	// std::ifstream ifs("config.txt");
	FILE* f;
	f = fopen("config.txt", "r");
	if (!f) return -1;
	char name[256];
	fscanf(f, "%s", name);
	std::string filename = name;
	// ifs >> filename;
	int hwndInt;
	// ifs >> hwndInt;
	fscanf(f, "%x", &hwndInt);
	printf("%x\n", hwndInt);
	fclose(f);
	for (int i = 0; i < 2000; ++i)
	{
		std::stringstream str;
		str << filename;
		str << i;
		str << ".bmp";
		SaveWindow2BMP(HWND(hwndInt), str.str());
		Sleep(1000);
	}
	return 0;
}


int main()
{
	make_many_pictures();
	return 0;
}