#include <enet/enet.h>
#include "Menu.h"
#include "Server.h"
#include "Client.h"

int main()
{
	Menu menu({ "Start Server", "Start Client"});
	menu.DisplayPrompt();

	Server s;
	Client c;

	switch (menu.GetSelectedOption())
	{
	case 1:
		s.StartServer();
		break;
	case 2:
		c.StartClient();
		break;
	default:
		break;
	}
}

