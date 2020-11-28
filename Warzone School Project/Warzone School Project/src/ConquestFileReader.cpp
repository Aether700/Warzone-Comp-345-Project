#include "ConquestFileReader.h"
#include <fstream>

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

	// ConquestFileReader  ////////////////////////////////////////////////////////////////////

		//	converter from string numbers to integers **** this is a helper function
	int stringToInt(string s) {		//	takes the string s representing the number
		if (s.size() == 1)			//	if the string is a single char
			return (int)s[0] - 48;	//		convert it according the ASCII value
		else						//	if the number is represented on a multi
			return (10 * (stringToInt(s.substr(0, (s.size() - 1))))) + (int)s[(s.size() - 1)] - 48;
	}

	ConquestFileReader::ConquestFileReader() {}
	ConquestFileReader::ConquestFileReader(const ConquestFileReader& mapL)
		: territories(mapL.territories), continents(mapL.continents), borders(mapL.borders)
	{
		//	No need for copying because the data used is cleared after the map creation -
		//			which all take place inside the mapGenerator function.
		//	The only time this object stores data is durring the process of creating a map.
	}

	ConquestFileReader& ConquestFileReader::operator=(const ConquestFileReader& other) {
		//	No need for copying because the data used is cleared after the map creation -
		//			which all take place inside the mapGenerator function.
		//	The only time this object stores data is durring the process of creating a map.
		continents = other.continents;
		territories = other.territories;
		borders = other.borders;
		return *this;
	}

	std::ostream& ConquestFileReader::operator<<(std::ostream& stream)
	{
		stream << "ConquestFileReader";
		return stream;
	}

	//	goes through the file and stores the usefull data (continents, territories and borders) into the given vector arguments
	void ConquestFileReader::conquestParserFunction(const string& s, vector<Continent*>& continents, vector<Territory*>& countries, vector<Borders>& borders) {
		string line, section = "";			
		std::ifstream open_map(s);
		vector<vector<string>> *borders_string = new vector<vector<string>>;
		int id = 0;

		std::getline(open_map, line);
		while (!open_map.eof()) {

			std::getline(open_map, line);

			if (line == "")
			{
				continue;
			}
			if (line == "[Continents]") {
				section = "continents";
				continue;
			}
			else if (line == "[Territories]") {
				section = "territories";
				continue;
			}

			if (section == "continents") {
				int delimiter, bonus;
				string cont;
				delimiter = line.find("=");
				cont = line.substr(0, delimiter);
				bonus = stringToInt(line.substr(++delimiter, line.size() - 1));
				continents.push_back(new Continent(cont, bonus));
			}
			else if (section == "Territories") {
				int comma;						//	index value usefull to navigate through the line
				string name, continentName;
				comma = line.find(",");			//	first comma - preceeded by the name of the territory
				name = line.substr(0, comma);	//	saving the name of the territory
				line = line.substr(++comma);	//	dumping the string portion that was proccesed
				
				//	this section will make us jump over x and y points, which are not important.
				comma = line.find(",");
				line = line.substr(++comma);
				comma = line.find(",");
				line = line.substr(++comma);
				
				comma = line.find(",");			//	fourth comma - preceeded by the name of the continent
				continentName = line.substr(0, comma);	//	saving the name of the continent
				line = line.substr(++comma);
				
				//	starting this point will follow the borders
				vector<string> b;				//	temporary storry for borders
				while (comma != -1) {			//	unknown number of borders, so must loop untill no more comma's found
					comma = line.find(",");
					if (comma == -1) {
						b.push_back(line);
					}
					else {
						b.push_back(line.substr(0, comma));		//	saving each substring between 2 commas
					}
					line = line.substr(++comma);
				}
				borders_string->push_back(b);	//	saving the state of neigbouring of each territory
				
				//	make a new territory and push it in the territory vector
				Territory* t = new Territory(name, id++);
				territories.push_back(t);
				
				//	getting a pointer to the continent that holds this territory
				for (Continent* c : continents) {
					if (c->getName() == continentName) {
						t->setContinent(c);			//	updating the territory
						c->addTerritory(t);			//	updating the continent
					}
				}
				delete t;			//	free memory
				t = NULL;
			}
		}

		//	processing the double vector of names of bordering countries into double vector of id's 
		vector<vector<string>>::iterator row;		//	creating an iterator for rows
		vector<string>::iterator col;				//	creating an iterator for columns
		int index = 0;								//	index to be added as territory ID
		for (row = borders_string->begin(); row != borders_string->end(); row++) {	//	looping through the rows
			Borders* b = new Borders();				//	for each row we create a Borders object
			b->setBorders(index++);					//	first element of Borders object is the index of the territory it represents
			for(col = row->begin(); col != row->end(); col++) {		//	looping through each element of the row - these are strings 
																		//	and representing the names of the neigboring territories
				for (int i = 0; i < territories.size(); i++) {		//	looking at every territory
					if (territories[i]->getName() == *col) {		//	and check to match the name we have with the name of the territory
						b->setBorders(i);								//	populating the borders data member
						break;
					}
				}
			}
			borders.push_back(*b);					//	for each loop representing each territory - pushing into the borders
			delete b;								//	free memory
			delete borders_string;					//	free memory
			b = NULL;
			borders_string = NULL;
		}
	}
}
