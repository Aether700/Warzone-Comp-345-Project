#include "MapLoader.h"
#include <iostream>
using std::cout;
using std::endl;

void MapLoaderDriver() {
	WZ::MapLoader* loader = new WZ::MapLoader();
	WZ::Map* map;

	//	wrong maps
	std::cout << "generating wrong map from arcticWrong.txt" << std::endl;
	map = loader->mapGenerator("assets/map files/arcticWrong.txt");
	std::cout << "generating wrong map from bigeuropeWrong.map" << std::endl;
	map = loader->mapGenerator("assets/map files/bigeuropeWrong.map");
	
	// good maps
	std::cout << "generating correct map from solar.map" << std::endl;
	map = loader->mapGenerator("assets/map files/solar.map");
	cout << "printing map:\n" << *map << endl;

	std::cout << "generating correct map from MiddleEast-Qatar.txt" << std::endl;
	map = loader->mapGenerator("assets/map files/MiddleEast-Qatar.txt");
	cout << "printing map ... \n\n" << *map << endl;

	delete map;
	delete loader;
}