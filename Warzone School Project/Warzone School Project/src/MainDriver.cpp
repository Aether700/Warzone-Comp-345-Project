#include "Utils.h"
#include "GameEngine.h"

extern void gameObserversDriver();
extern void mainGameLoopDriver();
extern void startupPhaseDriver();
extern void OrdersDriver();
extern void CardsDriver();


int main()
{
	std::array<const char*, 5> choices = {"Observers", "Main Game Loop", "Startup Phase", "Orders", "Cards"};

	bool done = false;
	while(!done)
	{
		switch (WZ::AskInput(choices, "Exit"))
		{
		case 1:
			gameObserversDriver();
			break;

		case 2:
			mainGameLoopDriver();
			break;

		case 3:
			startupPhaseDriver();
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

