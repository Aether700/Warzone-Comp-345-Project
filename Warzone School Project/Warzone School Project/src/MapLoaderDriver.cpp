#include "MapLoader.h"
#include "Utils.h"
#include <iostream>

WZ::MapLoader* ChooseMapType()
{
	std::array<const char*, 2> choices = { "Domination Format", "Conquest File" };
	
	std::cout << "Which type of map do you want to load?\n";

	switch (WZ::AskInput(choices, "Back"))
	{
	case 1:
		return new WZ::MapLoader();

	case 2:
		return new WZ::ConquestFileReaderAdapter();

	case -1:
		return nullptr;
	}
}

void MapLoaderDriver() 
{
	WZ::MapLoader* loader = ChooseMapType();
	while (loader != nullptr)
	{
		WZ::Map* map = loader->mapGenerator();

		if (map == nullptr)
		{
			std::cout << "The map file provided is not valid. Please choose another one.\n";
		}
		else
		{
			std::cout << *map << "\n";
		}

		loader = ChooseMapType();
	}
}