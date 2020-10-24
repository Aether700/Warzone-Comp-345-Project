/* We've asked the teacher permission to make a Utils file and he allowed us to do it.
*/


#include "Utils.h"
#include <iostream>
#include <time.h>
#include <random>
#include <sstream>
#include <assert.h>

namespace WZ
{
	/*initializes the seed of the rand function using the standard srand function 
	by passing it the current time (time(NULL) returns the current time)
	*/
	void Random::Init()
	{
		srand(time(nullptr));
	}

	//wrapper function for the c function rand
	int Random::GetInt() { return rand(); }

	/* takes the value returned by rand as a float and divides it by 
		32000.0f (this specific value was obtained through trial and errors
	*/
	float Random::GetFloat()
	{
		return (float)rand() / 32000.0f;
	}

	int AskInput(const std::vector<std::string>& choices, const char* backMessage)
	{
		bool hasBackMessage = backMessage != "";


		for (size_t i = 0; i < choices.size(); i++)
		{
			std::cout << (i + 1) << "." << choices[i] << "\n";
		}

		if (hasBackMessage)
		{
			std::cout << (choices.size() + 1) << "." << backMessage << "\n";
		}

		while (true)
		{
			char input[100];

			std::cin >> input;

			while (input == "")
			{
				std::cin >> input;
			}

			int choice = atoi(input);

			if (hasBackMessage && choice == choices.size() + 1)
			{
				return -1;
			}
			else if (choice > 0 && choice <= choices.size())
			{
				return choice;
			}
			else
			{
				int lastIndex = choices.size();
				if (hasBackMessage)
				{
					lastIndex++;
				}

				std::cout << "Please Enter a Number between 1 & " << lastIndex << "\n";
			}
		}
	}

	bool AskYN()
	{
		std::array<const char*, 2> ynArr = { "Yes", "No" };
		return AskInput(ynArr) == 1;
	}

	float Clamp(float min, float max, float value)
	{
		if (value < min)
		{
			return min;
		}
		else if (value > max)
		{
			return max;
		}
		return value;
	}

	int Clamp(int min, int max, int value)
	{
		if (value < min)
		{
			return min;
		}
		else if (value > max)
		{
			return max;
		}
		return value;
	}

	int AskInt()
	{
		while (true)
		{
			std::string str;
			std::cin >> str;

			int value = atoi(str.c_str());

			if (value == 0 && str != "0")
			{
				std::cout << "Please Enter a Valid Number\n";
			}
			else
			{
				return value;
			}
		}
	}


	static std::string FitInTable(std::string_view str, unsigned int totalSpaces)
	{
		assert(str.size() <= totalSpaces);

		int spacesLeft = totalSpaces - str.size();

		int spacesBefore = spacesLeft / 2;
		int spacesAfter = spacesLeft - spacesBefore;
		std::stringstream ss;

		for (int i = 0; i < spacesBefore; i++)
		{
			ss << " ";
		}

		ss << str;

		for (int i = 0; i < spacesAfter; i++)
		{
			ss << " ";
		}

		return ss.str();
	}

	static size_t FindMaxSpaceColumn(std::string* table, size_t width, size_t height, size_t column)
	{
		size_t max = 0;

		for (size_t i = 0; i < height; i++)
		{
			size_t index = column + width * i;
			std::string& curr = table[index];
			if (table[index].size() > max)
			{
				max = table[index].size();
			}
		}
		return max;
	}

	static std::string SetTableEntry(std::string* table, size_t width, size_t height, size_t row, size_t* spaceArr)
	{
		assert(width != 0);

		std::stringstream ss;
		ss << "|";

		for (size_t i = 0; i < width; i++)
		{
			ss << FitInTable(table[i + width * row], spaceArr[i]) << "|";
		}

		ss << "\n";
		return ss.str();
	}

	static std::string DrawLine(size_t space)
	{
		assert(space > 0);
		std::stringstream ss;
		for (size_t i = 0; i < space; i++)
		{
			ss << "-";
		}
		ss << "\n";
		return ss.str();
	}

	std::string DrawTable(std::string* table, size_t width, size_t height)
	{
		assert(table != nullptr);
		assert(width != 0);
		assert(height != 0);

		//measure space needed for column width for each column
		size_t* colSpaces = new size_t[width];

		for (size_t i = 0; i < width; i++)
		{
			colSpaces[i] = FindMaxSpaceColumn(table, width, height, i);
		}

		//calculate total table width
		size_t totalSpace = 1;

		for (size_t i = 0; i < width; i++)
		{
			totalSpace += colSpaces[i] + 1;
		}

		//build table in stringstream
		std::stringstream ss;

		//header
		ss << DrawLine(totalSpace);
		ss << SetTableEntry(table, width, height, 0, colSpaces);
		ss << DrawLine(totalSpace);

		//entries
		for (size_t i = 1; i < height; i++)
		{
			ss << SetTableEntry(table, width, height, i, colSpaces);
		}

		delete[] colSpaces;

		ss << DrawLine(totalSpace);


		return ss.str();
	}

}