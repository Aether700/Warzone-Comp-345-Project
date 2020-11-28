#pragma once
#include <string>
#include <vector>
#include "Map.h"
using std::vector;
using std::string;

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
			Overloading function for the assignment operator
		*/
		ConquestFileReader& operator=(const ConquestFileReader&);
		/*
			Overloading function for the insertion operator
		*/
		std::ostream& operator<<(std::ostream&);
		/*
			extracts the needed data from the passed map file and stores it into the passed vectors acordingly
		*/
		void conquestParserFunction(const string&, vector<Continent*>&, vector<Territory*>&, vector<Borders>&);


	public:
		ConquestFileReader();										//	default constructor
		ConquestFileReader(const ConquestFileReader&);				//	parameterised constructor
		ConquestFileReader& operator=(const ConquestFileReader&);	//	assign operator overwriter
	};
}
