#pragma once
#include <vector>
#include <string>

using std::vector; using std::string;


// Menu class
// Example usage:
// 
// int option;
// 
// Menu("Start Client", "Start Server");
// Menu.DisplayPrompt();
// Menu.GetSelectedOption();
// option = Menu.GetSelectedOption();
// 

class Menu
{
	vector<string> m_options;
	int m_selectedOption = -1;

	int ConvertAndValidateInput(string input);
	void PrintOptions();
	void PrintPrompt();
	void GetInput();

public:
	Menu(vector<string> options) :
		m_options(options)
	{};
	void DisplayPrompt();
	int GetSelectedOption() { return m_selectedOption; };

};

