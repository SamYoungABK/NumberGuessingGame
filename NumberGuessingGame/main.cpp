#include <enet/enet.h>
#include <iostream>
#include "Menu.h"

using std::cout; using std::endl;

void InitializeEnet();

int main()
{
	InitializeEnet();

	Menu menu({ "Start Server", "Start Client"});
	menu.DisplayPrompt();
	cout << menu.GetSelectedOption();
}

void InitializeEnet()
{
	if (enet_initialize() < 0)
	{
		cout << "Failed to initialize enet." << endl;
		exit(EXIT_FAILURE);
	}
}