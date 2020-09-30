/*****************************************************************************************
Map loader class enables the player to select (choose from a list and load) the desired
map for the Risk game. 
The map format that are readable - .txt or .map exclusevly.
******************************************************************************************/
//	After the map is chosen, the map loader will verify the validity of the file,
// parse the file content and create a map object that will be used to play the game.

#pragma once
#include <iostream>
//#include "Map.h"
#include <fstream>

namespace WZ
{
	using std::ifstream;
	class MapLoader
	{
	private:
		ifstream& in;
		bool parserFunction(string s);

	public:
		explicit MapLoader();		//	default constructor
		~MapLoader();				//	default destructor
		string menu_loader();		//	menu function to navigate through the menu option
	};
}
