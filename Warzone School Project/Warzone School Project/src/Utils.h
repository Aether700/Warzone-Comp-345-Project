/* We've asked the teacher permission to make a Utils file and he allowed us to do it.
*/


#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <string>

namespace WZ
{
	/* Basic rudementary Random class to get random ints and floats
	*/
	class Random
	{
	public:
		/*	Initializes the Random class by setting the seed of the c 
			function rand to the current time allowing pseudo random number generation

			Needs to be used at the start of the program to set the seed. Do not use more than once. 
			only use when starting your program
		*/
		static void Init();

		/*	returns a random integer between 0 and RAND_MAX 
			(a maximum value defined by c libraries which is at least 32767). 
			Note that RAND_MAX is not cannot be returned

			returns: a random integer between 0 and RAND_MAX (RAND_MAX not included)
		*/
		static int GetInt();

		/* returns a random float number between 0 and 1 (one not included)

			returns: a random float number between 0 and 1 (one not included)
		*/
		static float GetFloat();
		
	};

	/*	takes an array of c string of any size and optional back message 
		to provide a "back" option to the user, displays those choices (and the optional back message if there is any)
		and takes input from the user to get their choice. Returns the number of the option (not it's index) or
		-1 if the user choose the back option. 

		Note that the function does not prompt the user (i.e. it does not ask any question) it only provides a 
		list of elements from which the user can pick a choice and returns the number of the element choosen 
		(counting from 1 not 0, so not an index) 

		To get the index inside the array add -1 to the int returned.

		choices: the array of c string representing the choices offered to the user.
		backMessage: the optional back message. will add an extra option for the user if a message is provided.

		returns: the number of the choice picked by the user (not it's index in the array) 
		or -1 if a back message was provided and the user choose it
	*/
	template<size_t Size>
	int AskInput(std::array<const char*, Size> choices, const char* backMessage = "")
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
			else if (choice > 0 && choice <= (int)choices.size())
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

	/*	takes a vector of std::string and optional back message	to provide a "back" option 
		to the user, displays those choices (and the optional back message if there is any)
		and takes input from the user to get their choice. Returns the number of the option (not it's index) or
		-1 if the user choose the back option.

		Note that the function does not prompt the user (i.e. it does not ask any question) it only provides a
		list of elements from which the user can pick a choice and returns the number of the element choosen
		(counting from 1 not 0, so not an index)

		To get the index inside the array add -1 to the int returned.

		choices: the vector of std::string representing the choices offered to the user.
		backMessage: the optional back message. will add an extra option for the user if a message is provided.

		returns: the number of the choice picked by the user (not it's index in the array)
		or -1 if a back message was provided and the user choose it
	*/
	int AskInput(const std::vector<std::string>& choices, const char* backMessage = "");

	/*	A Simplification of the generic AskInput function. It allows the user to pick between "yes" and "no" 
		returning true if they choose "yes" false if they choose "no"

		Note that the function does not prompt the user (i.e. it does not ask any question) it only provides the user with 
		the yes/no options and allows them to pick one of them

		returns: true if the user chose "yes" false if they chose "no"
	*/
	bool AskYN();

	/*	Clamps the value passed according to the minimum and maximum provided. Returns the "clamped" value i.e. 
		the min if value < min, the max if value > max or the value itself otherwise.

		min: the minimum value allowed for the float value to be clamped
		max: the maximum value allowed for the float value to be clamped
		value: the value that needs to be clamped.

		returns: min if value < min, max if value > max or value otherwise.
	*/
	float Clamp(float min, float max, float value);

	/*	Clamps the value passed according to the minimum and maximum provided. Returns the "clamped" value i.e.
		the min if value < min, the max if value > max or the value itself otherwise.

		min: the minimum value allowed for the int value to be clamped
		max: the maximum value allowed for the int value to be clamped
		value: the value that needs to be clamped.

		returns: min if value < min, max if value > max or value otherwise.
	*/
	int Clamp(int min, int max, int value);

	/*	takes input from the user making sure that the input provided is a valid int. 
		if the input is not a valid int the function will keep prompting the user until 
		a valid int value has been provided

		returns: a valid int value provided by the user
	*/
	int AskInt();

	/* returns a formatted string representation of the table provided

	  table: a one dimensional representation of the table to draw (table[i][j] == table[i + width * j])
	  width: the number of columns of the table
	  height: the number of rows of the table

	  returns: a formatted string representation of the table provided
	*/
	std::string DrawTable(std::string* table, size_t width, size_t height);
}