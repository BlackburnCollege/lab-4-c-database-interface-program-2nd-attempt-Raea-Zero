#include <iostream>
#include "sqlite3.h"
#include "menu_functions.h"
#include <string>

/*
* TO DO:
* Test every menu function
* When you submit, also find a way to send a picture of the simplified schema as a guide
*/

int main()
{
	sqlite3* DB;
	//these two just remake in each function
	char* errorMessage;
	int exit = sqlite3_open("save_file.db", &DB);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Creating/Opening Database" << std::endl;
	}

	int menuSelection = 0;
	std::string menuCatcher;
	do
	{
		std::cout << "Hello, welcome to the RPG save file database editor.\nPlease select an option from the menu:\n\n1) Create Database.\n2) Import Data.\n3) See Active Party Members.\n4) See All Party Members' Hit Points.\n5) Perform Custom SELECT Statement.\n6) Exit.\n\nYour selection: ";
		std::getline(std::cin, menuCatcher, '\n');
		menuSelection = std::stoi(menuCatcher);
		while (menuSelection < 1 || menuSelection > 6)
		{
			std::cout << "\nI'm sorry, that is an invalid selection.\nPlease choose a number from 1-6 corresponding to the menu options: ";
			std::getline(std::cin, menuCatcher, '\n');
			menuSelection = std::stoi(menuCatcher);
		}

		if (menuSelection == 1)
		{
			std::cout << "\n";
			createDatabase(DB);
			std::cout << "\n\n";
		}
		else if (menuSelection == 2)
		{
			std::cout << "\n";
			importData(DB);
			std::cout << "\n\n";
		}
		else if (menuSelection == 3)
		{
			std::cout << "\n";
			simpleSelect(DB);
			std::cout << "\n\n";
		}
		else if (menuSelection == 4)
		{
			std::cout << "\n";
			complexSelect(DB);
			std::cout << "\n\n";
		}
		else if (menuSelection == 5)
		{
			std::cout << "\n";
			userSelect(DB);
			std::cout << "\n\n";
		}
		else //option 6, exit the program and close the database file
		{
			std::cout << "\nGoodbye. Have a nice day!\n";
			sqlite3_close(DB);
			return 0;
		}
	} while (menuSelection != 6);
}

