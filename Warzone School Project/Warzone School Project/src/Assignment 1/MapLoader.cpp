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

	//  constructor
	explicit MapLoader::MapLoader() {
		map_name = menu_loader();
		if (map_validator(map_name)) {
			if (parserFunction(map_name, continents, countries, borders))
				Map newMap(continents, countries, borders);	//	to be done
		}
		else
			cout << "Invalid map file." << endl;
	}

	//  destructor
	MapLoader::~MapLoader() {
		cout << "The " << map_name << " has been unloaded." << endl;
	}

	class Continents {
	private:
		string name;
		int bonus;
	public:
		Continents(string name, int bonus) : name{ name }, bonus{ bonus }{}
		void setName(string name) {
			this->name = name;
		}
		string getName() const {
			return name;
		}
		void setBonus(int bonus) {
			this->bonus = bonus;
		}
		int getBonus() const {
			return bonus;
		}
	};

	class Countries {
	private:
		int id;
		string name;
		int continent;
	public:
		Countries(int id, string name, int cont) : id{ id }, name{ name }, continent{ cont }{}
		void setID(int id) {
			this->id = id;
		}
		int getID() const {
			return id;
		}

		void setName(string name) {
			this->name = name;
		}
		string getName() const {
			return name;
		}

		void setContinent(int continent) {
			this->continent = continent;
		}
		int getContinent() const {
			return continent;
		}
	};

	class Borders {
	private:
		vector<int> borders;			//	reprezents the border status of each country.
	public:
		void setBorders(int state) {
			borders.push_back(state);
		}
		int getBorder(int i) const {
			return borders[i];
		}
	};

	//  getters and setters
	void MapLoader::setMapName(const string& name) {
		map_name = name;
	}

	string MapLoader::getMapName() const {
		return map_name;
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

	void MapLoader::parserFunction(const string& s, vector<Continents>& continents, vector<Countries>& countries, vector<Borders>& borders) {
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
				continents.push_back(Continents(cont, bonus));
			}
			if (section.compare("countries")) {
				int space, id, continent;
				string name;
				space = line.find(" ");
				id = stringToInt(line.substr(0, space));
				line = line.substr(++space);
				space = line.find(" ");
				name = line.substr(0, space);
				line = line.substr(++space);
				continent = stringToInt(line.substr(0, space));
				countries.push_back(Countries(id, name, continent));
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