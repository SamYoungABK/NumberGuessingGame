#include <enet/enet.h>
#include <iostream>
#include "MainMenu.h"

using std::cout; using std::endl;

void InitializeEnet();

int main()
{
	InitializeEnet();
}

void InitializeEnet()
{
	if (!enet_initialize())
	{
		cout << "Failed to initialize enet." << endl;
		exit(EXIT_FAILURE);
	}
}