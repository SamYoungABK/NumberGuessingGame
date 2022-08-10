#include <enet/enet.h>
#include "Menu.h"
#include "Server.h"

int main()
{
	Menu menu({ "Start Server", "Start Client"});
	Server s;
	menu.DisplayPrompt();

	switch (menu.GetSelectedOption())
	{
	case '1':
		s.StartServer();
		break;
	default:
		break;
	}
}

