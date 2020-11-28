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
#include <iostream>
#include "Map.h"
#include "Utils.h"

using std::string;
using std::vector;
namespace WZ {

	/*
		Borders class object has a temporary character and is used to store each territory's neigbours - obtained from the map file.
		Based on this object, the map graph is created, followed by the borders object deletion.
	*/
	class Borders {
	private:
		vector<int> borders;			//	a list of borders for each territory - it reflects the neighbouring territories saved as ID in a list
	public:
		void setBorders(int state);		//	setter for borders
		vector<int>& getBorder();		//	getter for borders
	};

	class MapLoader
	{
	private:
		/*
			the continents object is a temporary storage container for the continent values taken from the map file.
		*/
		vector<Continent*> continents;
		/*
			the territories object is a temporary storage container for the territorie values taken from the map file.
		*/
		vector<Territory*> territories;
		/*
			the borders object is a temporary storage container for the border values taken from the map file.
		*/
		vector<Borders> borders;
		/*
			determine if the given file contains a map or not
		*/
		bool map_validator(const string&);
		/*
			extracts the needed data from the passed map file and stores it into the passed vectors acordingly
		*/
		void parserFunction(const string&, vector<Continent*>&, vector<Territory*>&, vector<Borders>&);
		/*
			function that creates the graph which will represent the map during the game play. All adjacency
		*/
		void setAdjList();

	public:
		MapLoader();										//	default constructor
		MapLoader(const MapLoader&);						//	parameterised constructor
		MapLoader& operator=(const MapLoader&);				//	assign operator overwriter
		Map* mapGenerator();								//	default map generator - user picks a map from the predefined map folder
		virtual Map* mapGenerator(const string&);					//	a map generator where the path of the map file and name is predefined
		string menu_loader(const string& path = "/Maps");	//	menu function to navigate through the menu option
	};
	/*
		overwriter of the insertion operator
	*/
	std::ostream& operator<<(std::ostream& stream, const MapLoader& m);


	class ConquestFileReader
	{
	private:
		/*
			the continents object is a temporary storage container for the continent values taken from the map file.
		*/
		vector<Continent*> continents;
		/*
			the territories object is a temporary storage container for the territorie values taken from the map file.
		*/
		vector<Territory*> territories;
		/*
			the borders object is a temporary storage container for the border values taken from the map file.
		*/
		vector<Borders> borders;
		/*
			determine if the given file contains a map or not
		*/
		bool conquest_map_validator(const string&);
		/*
			extracts the needed data from the passed map file and stores it into the passed vectors acordingly
		*/
		void conquestParserFunction(const string&, vector<Continent*>&, vector<Territory*>&, vector<Borders>&);
		/*
			function that creates the graph which will represent the map during the game play. All adjacency
		*/
		void setAdjList();

	public:
		ConquestFileReader();										//	default constructor
		ConquestFileReader(const ConquestFileReader&);				//	parameterised constructor
		ConquestFileReader& operator=(const ConquestFileReader&);	//	assign operator overwriter
		Map* conquestMapGenerator();								//	default map generator - user picks a map from the predefined map folder
		Map* conquestMapGenerator(const string&);					//	a map generator where the path of the map file and name is predefined
		string conquest_menu_loader(const string& path = "/Maps");	//	menu function to navigate through the menu option
	};


	///////////////////////////////////////////////////////////////////ConquestFileReaderAdapter///////////////////////////////////////////////////////////////////////

	class ConquestFileReaderAdapter :public MapLoader {
	private:
		ConquestFileReader* filereader;													// 	data member
	public:
		ConquestFileReaderAdapter();													//	default constructor
		~ConquestFileReaderAdapter();													//	default constructor
		ConquestFileReaderAdapter(const ConquestFileReaderAdapter& obj);				//	copy constructor
		ConquestFileReaderAdapter& operator=(const ConquestFileReaderAdapter& obj);		//	assignment operator
		virtual Map* mapGenerator(const string&) override;								//	function that returns the map with the file reader

	};

	std::ostream& operator<<(std::ostream& stream, const ConquestFileReaderAdapter& m);
}

