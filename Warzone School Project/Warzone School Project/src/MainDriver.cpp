#include "Utils.h"
#include "GameEngine.h"

extern void MapLoaderDriver();
extern void WZ::StrategyDriver();


int main()
{
	std::array<const char*, 2> choices = { "Player Strategy", "Adapter" };

	bool done = false;
	while(!done)
	{
		switch (WZ::AskInput(choices, "Exit"))
		{
		case 1:
			WZ::StrategyDriver();
			break;

		case 2:
			MapLoaderDriver();
			break;


		case -1:
			done = true;
			break;
		}
		std::cout << "\n";
	}
}

