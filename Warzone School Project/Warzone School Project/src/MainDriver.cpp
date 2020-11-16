#include "Utils.h"
#include "GameEngine.h"

extern void gameStartDriver();
extern void startupPhaseDriver();
extern void mainGameLoopDriver();
extern void OrdersDriver();
extern void gameObserversDriver();


int main()
{
	std::array<const char*, 5> choices = {"Game Start", "Startup Phase", "Main Game Loop", "Orders", "Observers"};

	bool done = false;
	while(!done)
	{
		switch (WZ::AskInput(choices, "Exit"))
		{
		case 1:
			gameStartDriver();
			break;

		case 2:
			startupPhaseDriver();
			break;

		case 3:
			mainGameLoopDriver();
			break;

		case 4:
			OrdersDriver();
			break;

		case 5:
			gameObserversDriver();
			break;

		case -1:
			done = true;
			break;
		}
		std::cout << "\n";
	}
}

