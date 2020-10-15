/* We've asked the teacher permission to make a Utils file and he allowed us to do it.
*/


#include "Utils.h"
#include <iostream>
#include <time.h>
#include <random>

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
}