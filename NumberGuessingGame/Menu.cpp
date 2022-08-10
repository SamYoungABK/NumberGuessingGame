#include "Menu.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector; using std::string;
using std::cout;  using std::endl;
using std::cin;
using std::getline;

// returns -1 if the input is invalid
int Menu::ConvertAndValidateInput(string userInput)
{
	int convertedInput;
	int userInputLength;
	char* input;
	
	userInputLength = strlen(userInput.c_str()) + 1;
	input = new char[userInputLength];

	strcpy_s(input, userInputLength, userInput.c_str());

	convertedInput = int(strtol(input, &(input)+sizeof(input), 0));
	
	if (convertedInput < 0 || convertedInput > m_options.size())
		return false;

	return convertedInput;
}

void Menu::PrintOptions()
{
	for (int i = 1; i <= m_options.size(); ++i)
		cout << i << ". " << m_options[i-1] << endl;
}

void Menu::PrintPrompt()
{
	static const int firstNumber = 1;
	static const int lastNumber = m_options.size();

	cout << endl;
	cout << '(';

	for (int i = firstNumber; i < lastNumber; ++i)
		cout << i << ", ";

	cout << lastNumber << "): ";
}

void Menu::GetInput()
{
	string userInput;
	getline(cin, userInput);

	m_selectedOption = ConvertAndValidateInput(userInput);
}

void Menu::DisplayPrompt()
{
	bool validInput = false;
	while(m_selectedOption == -1)
	{
		system("cls");
		PrintOptions();
		PrintPrompt();
		GetInput();
	}
}