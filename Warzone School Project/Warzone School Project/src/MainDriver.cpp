#include "Utils.h"
#include "GameEngine.h"

extern void MapDriver();
extern void mainGameLoopDriver();
extern void PlayerDriver();
extern void OrdersDriver();
extern void CardsDriver();


int main()
{
	std::array<const char*, 5> choices = {"Map", "Main Game Loop", "Player", "Orders", "Cards"};

	bool done = false;
	while(!done)
	{
		switch (WZ::AskInput(choices, "Exit"))
		{
		case 1:
			MapDriver();
			break;

		case 2:
			mainGameLoopDriver();
			break;

		case 3:
			PlayerDriver();
			break;

		case 4:
			OrdersDriver();
			break;

		case 5:
			CardsDriver();
			break;

		case -1:
			done = true;
			break;
		}
		std::cout << "\n";
	}
}

