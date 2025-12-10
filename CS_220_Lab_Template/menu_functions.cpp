#include "menu_functions.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

void createDatabase(sqlite3* DB)
{
	int exit = 0;
	char* errorMessage;
	//"create database save_file;"
	std::string createCommand = (
								"create table Item_Index"
								"("
								"item_ID integer primary key not null,"
								"name text not null,"
								"description text not null"
								");"
								"create table Inventory"
								"("
								"item integer primary key not null,"
								"equipped boolean not null,"
								"foreign key (item) references Item_Index(item_ID)"
								");"
								"create table Stats"
								"("
								"creature_ID integer primary key not null,"
								"hit_points integer not null,"
								"attack integer not null,"
								"defense integer not null,"
								"speed integer not null"
								");"
								"create table Character"
								"("
								"name text primary key not null,"
								"level integer not null,"
								"class text not null,"
								"active bool not null,"
								"equipment integer,"
								"battle_stats integer not null,"
								"foreign key (equipment) references Inventory(item),"
								"foreign key (battle_stats) references Stats(creature_ID)"
								");"
								);

	exit = sqlite3_exec(DB, createCommand.c_str(), NULL, 0, &errorMessage);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Create Database/Tables. Error message: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
	}
	else
	{
		std::cout << "Database and tables created successfully!";
	}
}

//menu option 2: import data
void importData(sqlite3* DB)
{
	int exit = 0;
	char* errorMessage;

	int recordNum; //to track the current record when reading the files; reset to zero before reading a file

	std::ifstream itemIndex;

	//vectors to hold data from each field
	std::vector<std::string> itemIDs;
	std::vector<std::string> itemNames;
	std::vector<std::string> itemDescriptions;

	itemIndex.open("item_index.csv");
	recordNum = 0;
	while (!itemIndex.eof())
	{
		//temporary holder strings for each value
		//I chose to keep the strings because of my backup check for an empty string. If the program misses the end of the file and loops like it has sometimes, I don't want the empty string going in a vector.
		std::string itemIDString;
		std::string itemNameString;
		std::string itemDescriptionString;

		std::getline(itemIndex, itemIDString, ',');
		if (itemIDString == "")
		{
			break;
		}
		//this line below crashes the program for some reason, something to do with the vector. Adding 1 to the index doesn't help, everything looks fine when debugging.
		itemIDs[recordNum] = itemIDString;
		std::getline(itemIndex, itemNameString, ',');
		itemNames[recordNum] = itemNameString;
		std::getline(itemIndex, itemDescriptionString, '\n');
		itemDescriptions[recordNum] = itemDescriptionString;

		recordNum++;
	}
	itemIndex.close();

	//insert the data into the table
	for (int index = 0; index < itemIDs.size(); index++)
	{
		std::string fillItemIndex = ("insert into Item_Index values(" + itemIDs[index] + ", '" + itemNames[index] + "', '" + itemDescriptions[index] + "');");
		//code here to execute the statement
		exit = sqlite3_exec(DB, fillItemIndex.c_str(), NULL, 0, &errorMessage);
		if (exit != SQLITE_OK)
		{
			std::cerr << "Error Importing Item_Index" << std::endl;
			sqlite3_free(errorMessage);
		}
		else
		{
			std::cout << "Item Index imported successfully!\n";
		}
	}

	std::ifstream inventory;

	std::vector<std::string> inventoryItems;
	std::vector<std::string> equippedFlags;

	inventory.open("inventory.csv");
	recordNum = 0;
	while (!itemIndex.eof())
	{
		//temporary holder strings for each value
		std::string inventoryItemsString;
		std::string equippedFlagsString;

		std::getline(inventory, inventoryItemsString, ',');
		if (inventoryItemsString == "")
		{
			break;
		}
		inventoryItems[recordNum] = inventoryItemsString;
		std::getline(inventory, equippedFlagsString, '\n');
		equippedFlags[recordNum] = equippedFlagsString;

		recordNum++;
	}
	inventory.close();

	for (int index = 0; index < inventoryItems.size(); index++)
	{
		std::string fillInventory = ("insert into Inventory values(" + inventoryItems[index] + ", " + equippedFlags[index] + ");");
		//code here to execute the statement
		exit = sqlite3_exec(DB, fillInventory.c_str(), NULL, 0, &errorMessage);
		if (exit != SQLITE_OK)
		{
			std::cerr << "Error Importing Inventory" << std::endl;
			sqlite3_free(errorMessage);
		}
		else
		{
			std::cout << "Inventory imported successfully!\n";
		}
	}

	std::ifstream stats;

	std::vector<std::string> creatureIDs;
	std::vector<std::string> hitPoints;
	std::vector<std::string> attacks;
	std::vector<std::string> defenses;
	std::vector<std::string> speeds;

	stats.open("stats.csv");
	recordNum = 0;
	while (!itemIndex.eof())
	{
		//temporary holder strings for each value
		std::string creatureIDString;
		std::string hitPointsString;
		std::string attackString;
		std::string defenseString;
		std::string speedString;

		std::getline(stats, creatureIDString, ',');
		if (creatureIDString == "")
		{
			break;
		}
		creatureIDs[recordNum] = creatureIDString;
		std::getline(stats, hitPointsString, ',');
		hitPoints[recordNum] = hitPointsString;
		std::getline(stats, attackString, ',');
		attacks[recordNum] = attackString;
		std::getline(stats, defenseString, ',');
		defenses[recordNum] = defenseString;
		std::getline(stats, speedString, '\n');
		speeds[recordNum] =speedString;

		recordNum++;
	}
	stats.close();

	for (int index = 0; index < creatureIDs.size(); index++)
	{
		std::string fillStats = ("insert into Stats values(" + creatureIDs[index] + ", " + hitPoints[index] + ", " + attacks[index] + ", " + defenses[index] + ", " + speeds[index] + ");");
		//code here to execute the statement
		exit = sqlite3_exec(DB, fillStats.c_str(), NULL, 0, &errorMessage);
		if (exit != SQLITE_OK)
		{
			std::cerr << "Error Importing Stats" << std::endl;
			sqlite3_free(errorMessage);
		}
		else
		{
			std::cout << "Stats imported successfully!\n";
		}
	}

	std::ifstream character;

	std::vector<std::string> characterNames;
	std::vector<std::string> characterLevels;
	std::vector<std::string> characterClasses;
	std::vector<std::string> characterActiveFlags;
	std::vector<std::string> characterEquipments;
	std::vector<std::string> characterStats;

	character.open("character.csv");
	recordNum = 0;
	while (!itemIndex.eof())
	{
		//temporary holder strings for each value
		std::string nameString;
		std::string levelString;
		std::string classString;
		std::string activeString;
		std::string equipmentString;
		std::string statsString;

		std::getline(character, nameString, ',');
		if (nameString == "")
		{
			break;
		}
		characterNames[recordNum] = nameString;
		std::getline(character, levelString, ',');
		characterLevels[recordNum] = levelString;
		std::getline(character, classString, ',');
		characterClasses[recordNum] = classString;
		std::getline(character, activeString, ',');
		characterActiveFlags[recordNum] = activeString;
		std::getline(character, equipmentString, ',');
		characterEquipments[recordNum] = equipmentString;
		std::getline(character, statsString, '\n');
		characterStats[recordNum] = statsString;

		recordNum++;
	}
	character.close();

	for (int index = 0; index < characterNames.size(); index++)
	{
		std::string fillCharacter = ("insert into Character values('" + characterNames[index] + "', " + characterLevels[index] + ", '" + characterClasses[index] + "', " + characterActiveFlags[index] + ", " + characterEquipments[index] + ", " + characterStats[index] + "); ");
		//code here to execute the statement
		exit = sqlite3_exec(DB, fillCharacter.c_str(), NULL, 0, &errorMessage);
		if (exit != SQLITE_OK)
		{
			std::cerr << "Error Importing Character" << std::endl;
			sqlite3_free(errorMessage);
		}
		else
		{
			std::cout << "Character imported successfully!\n";
		}
	}
}

//menu option 3: simple select
void simpleSelect(sqlite3* DB)
{
	int exit = 0;
	char* errorMessage;

	std::string simpleSelect("select * from character where active = true;");

	exit = sqlite3_exec(DB, simpleSelect.c_str(), displayData, 0, &errorMessage);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error SELECT" << std::endl;
		sqlite3_free(errorMessage);
	}
}

//menu option 4: complex select
void complexSelect(sqlite3* DB)
{
	int exit = 0;
	char* errorMessage;

	std::string complexSelect("select Character.name, Stats.hit_points from Character, Stats where Character.battle_stats = Stats.creature_ID;");

	exit = sqlite3_exec(DB, complexSelect.c_str(), displayData, 0, &errorMessage);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error SELECT" << std::endl;
		sqlite3_free(errorMessage);
	}
}

//menu option 5: user-defined select
void userSelect(sqlite3* DB)
{
	int exit = 0;
	char* errorMessage;

	std::string userSelect;

	std::cout << "Please enter your SQLite SELECT statement: ";
	std::getline(std::cin, userSelect, '\n');
	
	//execute the statement
	exit = sqlite3_exec(DB, userSelect.c_str(), displayData, 0, &errorMessage);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error SELECT" << std::endl;
		sqlite3_free(errorMessage);
	}
}

//from the exam, a callback function to send to SQL
static int displayData(void* data, int argc, char** argv, char** azColName)
{
	int i;
	
	for (i = 0; i < argc; i++)
	{
		std::cout << azColName[i] << " = ";
		if (argv[i] == NULL)
		{
			std::cout << "NULL\n";
		}
		else
		{
			std::cout << argv[i] << '\n';
		}
	}

	std::cout << "\n";
	return 0;
}