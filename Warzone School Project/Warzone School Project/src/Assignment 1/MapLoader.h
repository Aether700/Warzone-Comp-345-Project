/*****************************************************************************************
Map loader class enables the player to select (choose from a list and load) the desired
map for the Risk game.
The map format that are readable - .txt or .map exclusevly.
******************************************************************************************/
//	After the map is chosen, the map loader will verify the validity of the file,
// parse the file content and create a map object that will be used to play the game.

#pragma once
#include <string>
#include <vector>
//#include "Map.h"
using std::string;
using std::vector;
namespace WZ {

	class MapLoader
	{
	private:
		string map_name;
		vector<Continents> continents;
		vector<Countries> countries;
		vector<Borders> borders;
		bool map_validator(const string&);		//	determine if the file contains a map or not
		void parserFunction(const string&, vector<Continents>&, vector<Countries>&, vector<Borders>&);
	public:
		void setMapName(const string&);
		string getMapName() const;
		explicit MapLoader();						//	default constructor
		~MapLoader();								//	default destructor
		string menu_loader(const string& path = "\Maps");			//	menu function to navigate through the menu option
	}
}