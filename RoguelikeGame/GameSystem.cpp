#include "GameSystem.h"
#include "Item.h"
#include<string>
#include<sstream>
#include<iostream>
#include<conio.h>
#include<fstream>
#include<Windows.h>

//Constructor sets up the game
//Put in the player info to basically remake the player?
GameSystem::GameSystem()
{
	_ifDeath = false;
	//_currentFloor = 1;
	//_campaign = campaign;
	//ifstream file;;

	//file.open("saveFileNames.txt");

	//if (file.fail()) {
		//perror("saveFileNames.txt");
		//system("PAUSE");
		//exit(1);
	//}

	//string line;

	//while (getline(file, line)) {
		//_saveFileNames.push_back(line);
	//}

	//file.close();

	//_player.init(1, 100, 10, 10, 0, 50, 1.0, 100, 0, 3, 4);

	//_level.setNoteFileName(noteFileName);
	//_level.load(levelFileName, _player);
	//_level.print(_player);
}

void GameSystem::restart(string levelFileName, string noteFileName, string campaign, string playerName) {
	_currentFloor = 1;
	_campaign = campaign;
	_ifDeath = false;
	_level.resetLevel();
	_level.setIfDead(_ifDeath);
	//Make sure you've cleared everything
	_level.clearEnemies();
	_level.clearItems();
	_level.clearLevelData();
	_level.clearIfVisible();
	_level.clearVisibleLevelData();
	_player.clearInventory();
	_player.clearEquipment();

	_player.init(playerName, 1, 100, 10, 10, 0, 50, 1.0, 100, 0, 3, 6);

	_level.setNoteFileName(noteFileName);
	_level.load(levelFileName, _player);
	_level.print(_player);
}

void GameSystem::loadGame(string saveFileName, string &campaign, int &currentFloor) {
	_level.clearEnemies();
	_level.clearItems();
	_level.clearLevelData();
	_level.clearIfVisible();
	_level.clearVisibleLevelData();
	_player.clearInventory();
	_player.clearEquipment();
	_ifDeath = false;
	_level.setIfDead(_ifDeath);
	//So first you initialize the floor and campaign that the player is on

	//Then you initialize the player with his old stats and such 


	//Then you initialize his inventory and equipment
	//You find his inventory, and if it is equipped, then you switch the tile and you equip it
	//After that, you just load the game?

	//This  should  be in GameSystem btw
	//Loads the level
	ifstream file;

	file.open(saveFileName);
	if (file.fail()) {
		perror(saveFileName.c_str());
		system("PAUSE");
		exit(1);
	}

	file >> std::ws;

	string line;
	stringstream ss;

	string levelFileName, noteFileName;
	//in save file, you need current level, health, attack, defense, experience, experienceLeft
	//maxHealth, gold, sightRange, equipment, inventory, experienceMultiplier, campaign, floor

	//void init(int level, int health, int attack, int defense, int experience, int experienceLeft, int maxHealth, int gold, int inventorySpace, int sightRange);
	int index = 0;
	int gameSystemHoldIndex = 100, playerHoldIndex = 100, itemHoldIndex = 100;
	//Player variables
	string playerName;
	int level, health, attack, defense, experience, experienceLeft, maxHealth, gold, inventorySpace, sightRange;
	double experienceMultiplier;
	//Item variables
	string name;
	char tile;
	int value, sellValue;
	bool ifEquipped, ifEquipable;
	while (getline(file, line)) {
		istringstream ss(line);
		ss >> std::skipws;
		
		if (line == "GameSystem:") {
			gameSystemHoldIndex = index;
			//gets campaign
			//gets floor
		}
		if (index == gameSystemHoldIndex + 1)
			ss >> campaign;
		if (index == gameSystemHoldIndex + 2)
			ss >> currentFloor;

		if (line == "Player:") {
			playerHoldIndex = index;
			//gets level
			//health
			//attack
			//defense
			//experience
			//experienceLeft
			//experienceMultiplier
			//maxHealth
			//gold
			//inventorySpace
			//sightRange
			//IN THAT ORDER
		}
		if (index == playerHoldIndex + 1)
			//ss >> playerName;
			playerName = line;
		if (index == playerHoldIndex + 2)
			ss >> level;
		if (index == playerHoldIndex + 3)
			ss >> health;
		if (index == playerHoldIndex + 4)
			ss >> attack;
		if (index == playerHoldIndex + 5)
			ss >> defense;
		if (index == playerHoldIndex + 6)
			ss >> experience;
		if (index == playerHoldIndex + 7)
			ss >> experienceLeft;
		if (index == playerHoldIndex + 8)
			ss >> experienceMultiplier;
		if (index == playerHoldIndex + 9)
			ss >> maxHealth;
		if (index == playerHoldIndex + 10)
			ss >> gold;
		if (index == playerHoldIndex + 11)
			ss >> inventorySpace;
		if (index == playerHoldIndex + 12)
			ss >> sightRange;
		//Item(string name, char tile, int value, int sellValue, bool ifEquipable, bool ifEquipped);
		if (line == "Item:") {
			itemHoldIndex = index;
			//gets name
			//tile
			//value
			//sellValue
			//ifEquipable
			//ifEquipped (IF TRUE, also add to equipment by checking the tile
		}
		if (index == itemHoldIndex + 1)
			//ss >> name;
			name = line;
		if (index == itemHoldIndex + 2)
			ss >> tile;
		if (index == itemHoldIndex + 3)
			ss >> value;
		if (index == itemHoldIndex + 4)
			ss >> sellValue;
		if (index == itemHoldIndex + 5)
			ss >> ifEquipable;
		if (index == itemHoldIndex + 6) {
			ss >> ifEquipped;
			//initialize the item
			_player.addToInventory(Item(name, tile, value, sellValue, ifEquipable, ifEquipped));
			if (ifEquipped)
				_player.equip(_player.getInventoryItem(_player.getInventorySize() - 1));
			name = "none";
			tile = '.';
			value = sellValue = 0;
			ifEquipable = ifEquipped = false;
		}
		ss.str("");
		index += 1;
	}
	_campaign = campaign;
	_currentFloor = currentFloor;
	ss << "Levels/Level" << campaign << currentFloor << ".txt";
	//string s = ss.str();
	levelFileName = ss.str();
	ss.str("");
	ss << "Notes/Note" << campaign << currentFloor << ".txt";
	noteFileName = ss.str();
	ss.str("");

	_player.init(playerName, level, health, attack, defense, experience, experienceLeft, experienceMultiplier, maxHealth, gold, inventorySpace, sightRange);
	_level.setNoteFileName(noteFileName);
	_level.load(levelFileName, _player);
	_level.print(_player);
}

void GameSystem::newFloor(string levelFileName, string noteFileName) {

	_currentFloor += 1;

	_level.clearEnemies();
	_level.clearItems();
	_level.clearLevelData();
	_level.clearIfVisible();
	_level.clearVisibleLevelData();

	//This is TOTALLY where you should put the item shop buy/sell sequence
	shopScreen();
	saveScreen();
	//Save screen saying would you like to save the game?

	_level.setNoteFileName(noteFileName);
	_level.load(levelFileName, _player);
	_level.print(_player);
}


void GameSystem::playGame() {

	_ifDeath = false;
	string deathMessage = "";
	bool ifDead = false;
	_level.setIfDead(ifDead);

	bool isDone = false;
	//playing the floor, it means
	bool ifUse = false;
	bool ifDrop = false;
	//for the use and throw away functions

	while (!isDone) {
		_level.print(_player);
		deathMessage = _level.getIfDead(ifDead);
		if(!ifDead)
			playerMove(ifUse, ifDrop, isDone);
		//Change room if at exit
		//checkIfWon(&isDone);
		if (ifDead) {
			deathScreen(deathMessage);
			isDone = true;
		}
		if (isDone && !ifDead)
			continueScreen();
		
		// screen before the next one, but this should also be where the death screen is.
		
		if(!ifUse && !isDone)
			_level.updateEnemies(_player);
	}
}

void GameSystem::playerMove(bool &ifUse, bool &ifDrop, bool &isDone) {
	char input;
	if (!ifUse) {
		printf("Enter a move(w/s/a/d), use(u), throw away(t), or info(i): ");
		input = _getch();
	}
	else {
		if (!ifDrop)
			printf("Enter the number of the item you wish to use: ");
		else
			printf("Enter the number of the item you wish you throw away: ");
		input = _getch();
		input -= '0';
	}
	if (!ifUse && input == 'i')
		printInfo();
	else 
		_level.movePlayer(input, _player, ifUse, ifDrop, isDone);

}

void GameSystem::printInfo() {
	stringstream ss;
	string widthBorder;
	vector <string> info;
	system("CLS");

	ifstream file;

	file >> std::noskipws;

	file.open("info.txt");

	if (file.fail()) {
		perror("info.txt");
		system("PAUSE");
		exit(1);
	}

	string line;
	int height = 0, width = 0;

	while (getline(file, line)) {
		height += 1; 
		if (line.size() > width)
			width = line.size();
		info.push_back(line);
	}
	//printf("\n\n\n\n\n\n\n\n");
	//system("PAUSE");

	file >> std::skipws;
	file.close();


	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int widthSpace = windowWidth - width;
	int heightSpace = windowHeight - height - 1;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	widthSpace = windowWidth - width;
	heightSpace = windowHeight - height - 1;

	if (widthSpace < 1 || heightSpace < 1) {
		printf("\nText too large to fit the screen size, please readjust\n");
		system("PAUSE");
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if (widthSpace < 1 || heightSpace < 1) {
			//MoveWindow(GetConsoleWindow(), 0, 0, _screenWidth, _screenHeight, TRUE);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			widthSpace = windowWidth - width;
			heightSpace = windowHeight - height - 1;
			if (widthSpace < 1 || heightSpace < 1) {
				printf("\nText cannot be properly displayed\n");
				system("PAUSE");
				exit(1);
			}
		}
	}
	widthBorder = "";
	for (int i = 0; i < int(widthSpace / 2); i++)
		widthBorder += " ";
	for (int i = 0; i < int(heightSpace / 2); i++)
		printf("\n");

	for (int i = 0; i < height; i++)
		printf("%s%s\n", widthBorder.c_str(), info[i].c_str());

	for (int i = 0; i < heightSpace - int(heightSpace / 2); i++)
		printf("\n");
	/*for (int i = 0; i < 33; i++) {
		if (i == 0)
			ss << "Information: ";
		if (i == 2)
			ss << "S is a Snake,     g is a Goblin,     B is a Bandit,     D is a Dragon";
		if (i == 4)
			ss << "o is a health potion,     G is a satchel of gold coins,     b is a magic holding bag";
		if (i == 6)
			ss << "t is a sword,     Y is a set of armour,     W is a shield,     * is an artifact";
		if (i == 8)
			ss << "E is the exit,      @ is YOU!,     # is a wall,      . is an empty space";
		printf("%s\n", ss.str().c_str());
		ss.str("");
	}*/
	system("PAUSE");
}


void GameSystem::continueScreen() {
	_level.print(_player);
	system("PAUSE");
}

void GameSystem::saveScreen() {
	bool a = true, b = true, c = true;
	string response;
	string saveFileName;
	printf("Would you like to save the game?");
	while (a) {
		//system("CLS");
		cin >> response;
		if (response == "yes" || response == "Yes" || response == "ok" || response == "Ok" || response == "OK" || response == "Y" || response == "y") {
			a = false;
			printf("Choose a name for the file!");
			while (b) {
				//Check for repeats
				cin >> response;
				bool ifRepeat = false;
				for (unsigned i = 0; i < _saveFileNames.size(); i++) {
					if (response == _saveFileNames[i])
						ifRepeat = true;
				}

				if (ifRepeat) {
					printf("That name is already taken you dingus! Would you like to overwrite it?");
					while (c) {
						string boolResponse;
						cin >> boolResponse;
						if (boolResponse == "yes" || boolResponse == "Yes" || boolResponse == "ok" || boolResponse == "Ok" || boolResponse == "OK" || boolResponse == "Y" || boolResponse == "y") {
							saveGame(response);
							c = false;
							b = false;
							a = false;
						}
						else if (boolResponse == "No" || boolResponse == "no" || boolResponse == "N" || boolResponse == "n") {
							c = false;
							b = false;
							printf("Would you still like to save your game?");
						}
						else {
							printf("\"%s\" doesn't really mean anything to me", boolResponse.c_str());
						}
					}
				}

				else {
					saveGame(response);
					b = false;
					a = false;
				}

			}
		}
		else if (response == "No" || response == "no" || response == "N" || response == "n") {
			a = false;
		}
		else {
			printf("Just say yes or no, man, do you want to save the game?");
		}
	}
}


void GameSystem::saveGame(string saveFileName) {
	ofstream saveFile;
	saveFile.open("SaveFiles/" + saveFileName + ".txt");
	ofstream addToSaveFiles;

	addToSaveFiles.open("saveFileNames.txt", std::ios_base::app);

	addToSaveFiles << saveFileName << "\n";

	int itemHoldIndex = 0;
	int whichItem = 0;
	bool ifWritingItem = false;
	
	for (unsigned i = 1; i < (unsigned)18 + _player.getInventorySize() * (unsigned)6; i++) {
		if (i == 1)
			saveFile << "GameSystem:";
		if (i == 2)
			saveFile << _campaign;
		if (i == 3)
			saveFile << _currentFloor;
		if (i == 4)
			saveFile << "Player:";
		if (i == 5)
			saveFile << _player.getName();
		if (i == 6)
			saveFile << _player.getLevel();
		if (i == 7)
			saveFile << _player.getHealth();
		if (i == 8)
			saveFile << _player.getAttack();
		if (i == 9)
			saveFile << _player.getDefense();
		if (i == 10)
			saveFile << _player.getExperience();
		if (i == 11)
			saveFile << _player.getExperienceLeft();
		if (i == 12)
			saveFile << _player.getExperienceMultiplier();
		if (i == 13)
			saveFile << _player.getMaxHealth();
		if (i == 14)
			saveFile << _player.getGold();
		if (i == 15)
			saveFile << _player.getInventorySpace();
		if (i == 16)
			saveFile << _player.getSightRange();
		if (i >= 17) {
			if (_player.getInventorySize() > whichItem && !ifWritingItem) {
				itemHoldIndex = i;
				ifWritingItem = true;
				saveFile << "Item:";
			}
			if (ifWritingItem && i == itemHoldIndex + 1)
				saveFile << _player.getInventoryItem(whichItem).getName();
			if (ifWritingItem && i == itemHoldIndex + 2)
				saveFile << _player.getInventoryItem(whichItem).getTile();
			if (ifWritingItem && i == itemHoldIndex + 3)
				saveFile << _player.getInventoryItem(whichItem).getValue();
			if (ifWritingItem && i == itemHoldIndex + 4)
				saveFile << _player.getInventoryItem(whichItem).getSellValue();
			if (ifWritingItem && i == itemHoldIndex + 5)
				saveFile << _player.getInventoryItem(whichItem).getIfEquipable();
			if (ifWritingItem && i == itemHoldIndex + 6) {
				saveFile << _player.getInventoryItem(whichItem).getIfEquipped();
				ifWritingItem = false;
				whichItem += 1;
			}
		}
		saveFile << "\n";
	}
	saveFile.close();
}

void GameSystem::deathScreen(string deathMessage) {
	string widthBorder;

	string name = _player.getName();
	int nameSpace = 19 - name.length();
	string space1 = "", space2 = "";
	for (int i = 0; i < int(nameSpace / 2); i++)
		space1 += " ";
	for (int i = 0; i < nameSpace - int(nameSpace / 2); i++)
		space2 += " ";
	//system("CLS");
	//printf("\n\n\n\n\n\n\n\n\n");
	system("CLS");

	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	int width = 33;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int height = 15;

	int widthSpace = windowWidth - width;
	int heightSpace = windowHeight - height - 1;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	widthSpace = windowWidth - width;
	heightSpace = windowHeight - height - 1;

	if (widthSpace < 1 || heightSpace < 1) {
		printf("\nText too large to fit the screen size, please readjust\n");
		system("PAUSE");
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if (widthSpace < 1 || heightSpace < 1) {
			//MoveWindow(GetConsoleWindow(), 0, 0, _screenWidth, _screenHeight, TRUE);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			widthSpace = windowWidth - width;
			heightSpace = windowHeight - height - 1;
			if (widthSpace < 1 || heightSpace < 1) {
				printf("\nText cannot be properly displayed\n");
				system("PAUSE");
				exit(1);
			}
		}
	}
	widthBorder = "";
	for (int i = 0; i < int(widthSpace / 2); i++)
		widthBorder += " ";
	for (int i = 0; i < int(heightSpace / 2); i++)
		printf("\n");
	_ifDeath = true;
	
  printf("%s                   /)\n", widthBorder.c_str());
  printf("%s               mo / )\n", widthBorder.c_str());
  printf("%s               |/)\\)\n", widthBorder.c_str());
  printf("%s                /\\_\n", widthBorder.c_str());
  printf("%s                \\__|=\n", widthBorder.c_str());
  printf("%s               (    )\n", widthBorder.c_str());
  printf("%s               __)(__\n", widthBorder.c_str());
  printf("%s         _____/      \\\\_____\n", widthBorder.c_str());
  printf("%s        |  _     ___   _   ||\n", widthBorder.c_str());
  printf("%s        | | \\     |   | \\  ||\n", widthBorder.c_str());
  printf("%s        | |  |    |   |  | ||\n", widthBorder.c_str());
  printf("%s        | |_/     |   |_/  ||\n", widthBorder.c_str());
  printf("%s        | | \\     |   |    ||\n", widthBorder.c_str());
  printf("%s        | |  \\    |   |    ||\n", widthBorder.c_str());
  printf("%s        | |   \\. _|_. | .  ||\n", widthBorder.c_str());
  printf("%s        |                  ||\n", widthBorder.c_str());
  printf("%s        |%s%s%s||\n", widthBorder.c_str(), space1.c_str(), name.c_str(), space2.c_str());
  printf("%s        |                  ||\n", widthBorder.c_str());
  printf("%s*       | *   **    * **   |**      **\n", widthBorder.c_str());
  printf("%s \\))ejm97/.,(//,,..,,\\||(,,.,\\\\,.((//\n", widthBorder.c_str());


  for (int i = 0; i < heightSpace - int(heightSpace / 2) - 1; i++)
	  printf("\n");

  widthBorder = "";
  for (int i = 0; i < int(windowWidth/2) - int(deathMessage.size()/2); i++) {
	  widthBorder += " ";
  }

  printf("%s%s\n", widthBorder.c_str(), deathMessage.c_str());
  
  widthBorder = "";

  for (int i = 0; i < int(windowWidth/2) - 16; i++)
	  widthBorder += " ";

  printf("%s", widthBorder.c_str());
  
  system("PAUSE");
	
}

void GameSystem::shopScreen() {
	bool ifShopping = true;
	bool ifBuy = false, ifSell = false, ifJoke = false;
	char input;
	int response;
	string shopResponse = "";
	stringstream ss1,ss2,ss3,ss4;
	string border = "       ";
	string bigBorder = "                                      ";
	string ifOn = "";
	int power = 3 + _player.getLevel();
	int healthPower = 25 * _player.getLevel();

	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	int width = 127;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int x = 0;

	ss1 << "a Strength " << power << " Sword";
	ss2 << "a Strength " << power << " Shield";
	ss3 << "a Strength " << power << " Armour";
	ss4 << "a Strength " << healthPower << " Health Potion";

	//Add scrolls and lanterns
	vector <Item> availableItems = {
		Item(ss4.str(), 'o', healthPower, 4*healthPower, false, false),
		Item("a magic bag for carrying things", 'b', 1, 250, false, false),
		Item("an ancient scroll", 's', healthPower, 5*healthPower, false, false),
		Item("a magic lantern", 'l', 1, 500, false, false),
		Item(ss1.str(), 't', power, 40 * power, true, false),
		Item(ss2.str(), 'W', power, 30 * power, true, false),
		Item(ss3.str(), 'Y', power, 50 * power, true, false)
	};// ss1.str(""); ss2.str(""); ss3.str("");

	int height = availableItems.size() + _player.getInventorySize() + 10;

	int widthSpace = windowWidth - width;
	int heightSpace = windowHeight - height - 1;

	//Use same centering technique as from level.

	while (ifShopping) {
		system("CLS");
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		widthSpace = windowWidth - width;
		heightSpace = windowHeight - height - 1;

		if (widthSpace < 1 || heightSpace < 1) {
			printf("\nText too large to fit the screen size, please readjust\n");
			system("PAUSE");
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			if (widthSpace < 1 || heightSpace < 1) {
				//MoveWindow(GetConsoleWindow(), 0, 0, _screenWidth, _screenHeight, TRUE);
				SetWindowPos(GetConsoleWindow(), 0, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
				SetWindowPos(GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
				windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
				windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
				widthSpace = windowWidth - width;
				heightSpace = windowHeight - height - 1;
				if (widthSpace < 1 || heightSpace < 1) {
					printf("\nText cannot be properly displayed\n");
					system("PAUSE");
					exit(1);
				}
			}
		}

		bigBorder = "";
		for (int i = 0; i < int(widthSpace / 2); i++)
			bigBorder += " ";
		for (int i = 0; i < int(heightSpace / 2); i++)
			printf("\n");
		for (int i = 0; i < height; i++) {
			printf("%s", bigBorder.c_str());
			if (i == 0)
				printf("**********************Welcome to the shop!!!*****************************              You have %i gold", _player.getGold());
			if (i == 2)
				printf("You can buy anything we have, sell anything you have, or get out of here!");
			if (i == 4)
				printf("Here's what we have in stock right now...");
			if (i >= 6 && (unsigned)i < availableItems.size() + 6)
				printf("%s%i. %s for only %i gold", border.c_str(), i - 5, availableItems[i - 6].getName().c_str(), availableItems[i - 6].getSellValue());
			/*
			if (i == 6)
				printf("%s1. a Health Potion to restore %i health for only %i gold", border.c_str(), healthPower, 4 * healthPower);
			if (i == 7)
				printf("%s2. a magic bag to increase your inventory size for only 250 gold", border.c_str());
			if (i == 8)
				printf("%s3. %s to increase your attack for only %i gold", border.c_str(), ss1.str().c_str(), availableItems[2].getSellValue());
			if (i == 9)
				printf("%s4. %s to increase your defense for only %i gold", border.c_str(), ss2.str().c_str(), availableItems[3].getSellValue());
			if (i == 10)
				printf("%s5. %s to increase your max health for only %i gold", border.c_str(), ss3.str().c_str(), availableItems[4].getSellValue());
				*/
			if (i == availableItems.size() + 7)
				printf("Here's what you have in your inventory...");
			if ((unsigned)i >= availableItems.size() + 9) {
				int index = i - availableItems.size() - 9;
				if (index < _player.getInventorySize()) {
					if (_player.getInventoryItem(index).getIfEquipable()) {
						if (_player.getInventoryItem(index).getIfEquipped())
							ifOn = "(ON)";
						else
							ifOn = "(OFF)";
					}
					else
						ifOn = "";
					printf("%s%i. %s %s and I can give you %i gold for it", border.c_str(), index + 1, _player.getInventoryItem(index).getName().c_str(), ifOn.c_str(), int(0.75*_player.getInventoryItem(index).getSellValue()));

				}
			}

				/*
			Item("a Health Potion", 'o', 10, 100, false, false));
			Item(ss.str(), 'G', randInt, randInt, false, false));
			Item("a magic bag for carrying things", 'b', 1, 250, false, false));
			Item(ss.str(), 't', int(randInt / 10), 20 * randInt, true, false));
			Item(ss.str(), 'W', int(randInt / 10), 10 * randInt, true, false));
			Item(ss.str(), 'Y', int(randInt / 5), 25 * randInt, true, false));
				Item("the Holy Grail", '*', 20, 500, true, false));
				Item("the Ring of Youth", '*', 25, 650, true, false));
				Item("a Really, Really Nice Hat", '*', 12, 100, true, false));
				Item("a Friendly Rat", '*', 15, 5, true, false));
				Item("the God Stone", '*', 22, 1000, true, false));
				Item("an Angry Ghoul", '*', 50, 500, true, false));
				*/

				printf("\n");
		}
		for (int i = 0; i < heightSpace - int(heightSpace / 2); i++)
			printf("\n");

		if (ifBuy) {
			if (x == 0) {
				printf("%sWhich numbered item would you like to buy? ", bigBorder.c_str());
				input = _getch();
				response = int(input - '0');
				if (response >= 1 && response < availableItems.size())
					shopResponse = _player.buyItem(availableItems[response - 1]);
				else
					shopResponse = "Stop goofing around!! ";
				if (shopResponse != "")
					x = 1;
				else
					ifBuy = false;
			}
			else {
				printf("%s%s", bigBorder.c_str(), shopResponse.c_str());
				_getch();
				x = 0;
				shopResponse = "";
				response = 0;
				ifBuy = false;
			}
		}
		else if (ifSell) {
			if (x == 0) {
				printf("%sWhich item would you like me to take off your hands?", bigBorder.c_str());
				input = _getch();
				response = int(input - '0');
				shopResponse = _player.sellItem(response - 1);
				if (shopResponse == "")
					ifSell = false;
				else
					x = 1;
			}
			else {
				printf("%s%s", bigBorder.c_str(), shopResponse.c_str());
				_getch();
				ifSell = false;
				x = 0;
				response = 0;
				shopResponse = "";
			}
		}
		else if (ifJoke) {
			x += 1;
			printf("%s", bigBorder.c_str());
			if(x == 1)
				printf("A bus carrying many people crashed on an icy road, burst into flames, and everyone died.");
			if (x == 2)
				printf("Upon arrival in heaven, God said, Since you have died in a terrible way, I\'ll grant you one wish before I let you into heaven.");
			if (x == 3)
				printf("The first woman, being a person always concerned on her looks, comes up to God and says, I wish to be beautiful.");
			if (x == 4)
				printf("God grants her wish. The next person can’t decide on what to wish for, so he ends up wishing for the same thing.");
			if (x == 5)
				printf("At this point a man at the very back of the line starts to laugh.");
			if (x == 6)
				printf("The next couple, seeing how utterly wondrous the two have become, make their wish to become beautiful also.");
			if (x == 7)
				printf("The man at the end laughs even louder. One after another, the people wish for the same thing.");
			if (x == 8)
				printf("The closer God gets to the end of the line, the harder the man laughs.");
			if (x == 9)
				printf("When God finally reaches him, he asks, What is your wish my son?");
			if (x == 10)
				printf("The man says, Make them all ugly again!");
			if (x == 11) {
				printf("HAHAHAHHAHAHahhahahAHHHAHahhhahahah :)");
				ifJoke = false;
				x = 0;
			}
				_getch();
		}
		else {
			printf("%sSo what'll it be? Buy(b), Sell(s), hear a cool joke(j), or leave(l)? ", bigBorder.c_str());
			input = _getch();
			switch(input){
			case 'b':
				ifBuy = true;
				x = 0;
				break;
			case 's':
				ifSell = true;
				x = 0;
				break;
			case 'j':
				ifJoke = true;
				x = 0;
				break;
			case 'l':
				ifShopping = false;
				break;
			default:
				break;
			}
		}
	}
}