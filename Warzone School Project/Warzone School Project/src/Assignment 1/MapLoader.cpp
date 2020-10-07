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

	class Borders {
	private:
		vector<int> borders;			//	reprezents the border status of each country.
	public:
		void setBorders(int state) {
			borders.push_back(state);
		}
		vector<int>& getBorder() {
			return borders;
		}
	};

	MapLoader::~MapLoader() {
		for (Borders b : borders)
			delete& b;
	}

	Map* MapLoader::mapGenerator(const string& map_name) {
		if (map_validator(map_name)) {
			parserFunction(map_name, continents, territories, borders);
			setAdjList();
			return new Map(continents);
		}
		else {
			cout << "Invalid map file." << endl;
			return NULL;
		}
	}

	Map* MapLoader::mapGenerator() {
		return mapGenerator(menu_loader());
	}

	void MapLoader::setAdjList() {
		for (Borders b : borders) {
			Territory* curentTerritory = territories[b.getBorder()[0] - 1];
			for (int i = 1; i < b.getBorder().size(); i++)
				curentTerritory->addAdjTerritory(territories[b.getBorder()[i] - 1]);
		}
	}

	bool MapLoader::map_validator(const string& map) {
		if (map.compare(""))                        //  checks for an empty folder
			return false;
		else {
			string line{ "" };                        //  string iterator through the file
			int valid[] = { 0, 0, 0 };              //  counter to determine if the map has all sections needed
			static std::ifstream in(map);           //  reader stream
			in.open(map);
			if (!in) {                              //  checks for corrupt files
				cout << "Unable to open file";
				return false;
			}
			while (std::getline(in, line)) {        //  checking line by line for the 3 sections needed
				if (line.compare("[continents]")) {
					valid[0]++;
				}
				if (line.compare("[countries]")) {
					valid[1]++;
				}
				if (line.compare("[borders]")) {
					valid[2]++;
				}
			}
			in.close();
			int correct[] = { 1, 1, 1 };
			if (valid != correct)             //  if any of the section is missing, 
				return false;
			return true;
		}
	}

	void MapLoader::parserFunction(const string& s, vector<Continent*>& continents, vector<Territory*>& countries, vector<Borders>& borders) {
		string line, section = "";
		std::ifstream open_map(s);
		open_map.open(s);
		while (std::getline(open_map, line)) {
			if (line.compare("")) continue;
			if (line.compare("[continents]")) {
				section = "continents";
				continue;
			}
			if (line.compare("[countries]")) {
				section = "countries";
				continue;
			}
			if (line.compare("[borders]")) {
				section = "borders";
				continue;
			}
			if (section.compare("continents")) {
				int space, bonus;
				string cont;
				space = line.find(" ");
				cont = line.substr(0, space);
				line = line.substr(++space);
				space = line.find(" ");
				bonus = stringToInt(line.substr(0, space));
				continents.push_back(new Continent(cont, bonus));
			}
			if (section.compare("countries")) {
				int space, id, continentIndex;
				string name;
				space = line.find(" ");
				id = stringToInt(line.substr(0, space));
				line = line.substr(++space);
				space = line.find(" ");
				name = line.substr(0, space);
				line = line.substr(++space);
				continentIndex = stringToInt(line.substr(0, space));
				Continent* c = continents[continentIndex - 1];
				Territory* t = new Territory(name, id, c);
				territories.push_back(t);
				t->setContinent(c);
				c->addTerritory(t);
			}
			if (section.compare("borders")) {
				Borders* b = new Borders();
				char* c = &line[0];
				string num = "";
				while (*c != '\n') {
					while (*c != ' ' || *c != '\n') {
						num += *c;
						c++;
					}
					b->setBorders(stringToInt(num));
					num = "";
					if (*c == ' ')
						c++;
				}
				borders.push_back(*b);
			}
		}
	}

	string MapLoader::menu_loader(const string& path) {
		string extension;
		vector<string> files;
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			extension = entry.path().filename().extension().string();
			if (extension.compare(".map") || extension.compare(".txt")) {
				files.push_back(entry.path().filename().string());
			}
		}
		if (files.size() == 0) {
			cout << "No map file were found." << endl;
			return "";
		}


		int fileIndex = AskInput(files);
		return files[fileIndex - 1];
	}

	int stringToInt(string s) {
		if (s.size() == 1)
			return (int)s[0] - 48;
		else
			return (10 * (stringToInt(s.substr(0, (s.size() - 1))))) + (int)s[(s.size() - 1)] - 48;
	}