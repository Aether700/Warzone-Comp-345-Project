#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <filesystem>
#include "MapLoader.h"
#include <vector>
#include <exception>
#include "Utils.h"


using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
namespace WZ {

	// Borders ////////////////////////////////////////////////////////////////////

	void Borders::setBorders(int state) {
		borders.push_back(state);
	}
	vector<int>& Borders::getBorder() {
		return borders;
	}

	// MapLoader //////////////////////////////////////////////////////////////////

	//	converter from string numbers to integers
	int stringToInt(string s) {		//	takes the string s representing the number
		if (s.size() == 1)			//	if the string is a single char
			return (int)s[0] - 48;	//		convert it according the ASCII value
		else						//	if the number is represented on a multi
			return (10 * (stringToInt(s.substr(0, (s.size() - 1))))) + (int)s[(s.size() - 1)] - 48;
	}

	MapLoader::MapLoader() {}
	MapLoader::MapLoader(const MapLoader& mapL) 
		: territories(mapL.territories), continents(mapL.continents), borders(mapL.borders) 
	{
		//	No need for copying because the data used is cleared after the map creation -
		//			which all take place inside the mapGenerator function.
		//	The only time this object stores data is durring the process of creating a map.
	}

	MapLoader& MapLoader::operator=(const MapLoader& other) {
		//	No need for copying because the data used is cleared after the map creation -
		//			which all take place inside the mapGenerator function.
		//	The only time this object stores data is durring the process of creating a map.
		continents = other.continents;
		territories = other.territories;
		borders = other.borders;
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const MapLoader& m) {
		stream << "MapLoader";
		return stream;
	}

	Map* MapLoader::mapGenerator(const string& map_name) {
		if (map_validator(map_name)) {
			parserFunction(map_name, continents, territories, borders);

			//make sure have enough territories for the number of borders
			if (territories.size() < borders.size())
			{
				cout << "Invalid map file." << endl;
				return NULL;
			}

			setAdjList();
			Map* mapPointer = new Map(continents);
			continents.clear();
			territories.clear();
			borders.clear();
			return mapPointer;
		}
		else {
			cout << "Invalid map file." << endl;
			return NULL;
		}
	}

	Map* MapLoader::mapGenerator() {
		return mapGenerator(menu_loader("assets/map files"));
	}

	void MapLoader::setAdjList() {
		for (Borders b : borders) {
			Territory* curentTerritory = territories[b.getBorder()[0] - 1];
			for (size_t i = 1; i < b.getBorder().size(); i++)
				curentTerritory->addAdjTerritory(territories[b.getBorder()[i] - 1]);
		}
	}

	bool MapLoader::map_validator(const string& map) {
		if (map == "")                        //  checks for an empty folder
			return false;
		else {
			string line{ "" };                        //  string iterator through the file
			int valid[] = { 0, 0, 0 };              //  counter to determine if the map has all sections needed
			std::ifstream in(map);			        //  reader stream
			
			if (!in) {                              //  checks for corrupt files
				cout << "Unable to open file";
				return false;
			}

			while (std::getline(in, line)) {        //  checking line by line for the 3 sections needed
				if (line == "[continents]") {
					valid[0]++;
				}
				else if (line == "[countries]") {
					valid[1]++;
				}
				else if (line == "[borders]") {
					valid[2]++;
				}
			}
			in.close();
			constexpr int correct[] = { 1, 1, 1 };
			for (int i = 0; i < sizeof(correct)/sizeof(int); i++)
			{
				if (valid[i] != correct[i])
				{
					return false;
				}
			}
			return true;
		}
	}

	void MapLoader::parserFunction(const string& s, vector<Continent*>& continents, vector<Territory*>& countries, vector<Borders>& borders) {
		string line, section = "";
		std::ifstream open_map(s);

		std::getline(open_map, line);
		while (!open_map.eof()) {
			
			std::getline(open_map, line);
			
			if (line == "")
			{
				continue;
			}
			if (line == "[continents]") {
				section = "continents";
				continue;
			}
			else if (line == "[countries]") {
				section = "countries";
				continue;
			}
			else if (line == "[borders]") {
				section = "borders";
				continue;
			}
			
			if (section == "continents") {
				int space, bonus;
				string cont;
				space = line.find(" ");
				cont = line.substr(0, space);
				line = line.substr(++space);
				space = line.find(" ");
				bonus = stringToInt(line.substr(0, space));
				continents.push_back(new Continent(cont, bonus));
			}
			else if (section == "countries") {
				int space, id, continentIndex;
				string name;
				space = line.find(" ");
				id = stringToInt(line.substr(0, space));
				line = line.substr(++space);
				space = line.find(" ");
				name = line.substr(0, space);
				line = line.substr(++space);
				space = line.find(" ");
				continentIndex = stringToInt(line.substr(0, space));
				Continent* c = continents[continentIndex - 1];
				Territory* t = new Territory(name, id, c);
				territories.push_back(t);
				t->setContinent(c);
				c->addTerritory(t);
			}
			else if (section == "borders") {
				Borders* b = new Borders();
				string num = "";
				

				for (size_t i = 0; i < line.size(); i++)
				{
					while (line[i] == ' ' && i < line.size()) { i++; }

					for (; line[i] != ' ' && line[i] != '\n' && i < line.size(); i++)
					{
						num += line[i];
					}

					b->setBorders(stringToInt(num));
					num = "";
				}

				borders.push_back(*b);
				delete b;
			}
		}
	}

	string MapLoader::menu_loader(const string& path) {
		string extension;
		vector<string> files;
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			extension = entry.path().filename().extension().string();
			if (extension == ".map" || extension == ".txt") {
				files.push_back(entry.path().filename().string());
			}
		}
		if (files.size() == 0) {
			cout << "No map file were found." << endl;
			return "";
		}


		int fileIndex = AskInput(files);
		std::cout << "\n";
		if (path == "")
		{
			return files[fileIndex - 1];
		}
		return path + "/" + files[fileIndex - 1];
	}
}	