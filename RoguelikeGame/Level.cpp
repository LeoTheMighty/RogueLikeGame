#include "Level.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<cstdio>
#include<random>
#include<ctime>
#include<Windows.h>


Level::Level()
{
	_floor = 0;
}


void Level::load(string fileName, Player &player) {

	_mapHeight = 0;
	_floor++;
	_currentStandingTile = '.';

	//Loads the level
	ifstream file;

	file.open(fileName);
	if (file.fail()) {
		perror(fileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;
	stringstream ss;

	while (getline(file, line)) {
		_mapHeight += 1;
		_levelData.push_back(line);
		_ifVisible.push_back({ });
		_visibleLevelData.push_back(line);
	}

	_mapWidth = _levelData[0].size();

	for (unsigned i = 1; i < _levelData.size(); i++) {
		if (_levelData[i].size() > (unsigned int)_mapWidth)
			_mapWidth = _levelData.size();
	}

	file.close();

	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	_screenWidth = desktop.right;
	_screenHeight = desktop.bottom;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	_windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	_windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	if (_windowHeight - _mapHeight < 6 || _windowWidth - _mapWidth < 39) {
		printf("\nMap too large to fit the screen size, please readjust\n");
		system("PAUSE");
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		_windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		_windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if (_windowHeight - _mapHeight < 6 || _windowWidth - _mapWidth < 39) {
			//MoveWindow(GetConsoleWindow(), 0, 0, _screenWidth, _screenHeight, TRUE);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, _screenWidth, _screenHeight, SWP_SHOWWINDOW);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			_windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			_windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			if (_windowHeight - _mapHeight < 6 || _windowWidth - _mapWidth < 39) {
				printf("\nMap cannot be played\n");
				system("PAUSE");
				exit(1);
			}
		}

	}

	//THE MAX NAME LENGTH IS 32, FOR EQUIPMENT it is 26

	//For the artifact choosing
	int decision = 0;

	//Process the level

	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> roll(0, 50 * player.getLevel());
	uniform_int_distribution<int> healthRoll(0, player.getLevel());
	uniform_int_distribution<int> artifactRoll(0, 5);
	uniform_int_distribution<int> levelRoll2(0, 2);
	uniform_int_distribution<int> levelRoll3(0, 3);
	uniform_int_distribution<int> levelRoll5(0, 5);
	uniform_int_distribution<int> levelRoll10(0, 10);
	int randInt = 0;

	int unknownRandInt = levelRoll10(randomEngine) + 20;

	uniform_int_distribution<int> unknownRoll(0, 25 * unknownRandInt);

	char tile;
	for (unsigned i = 0; i < _levelData.size(); i++) {
		for (unsigned j = 0; j < _levelData[i].size(); j++) {
			tile = _levelData[i][j];
			_ifVisible[i].push_back(false);
			

			switch (tile) {
			case '@': // Player
				player.setPosition(j, i);
				break;
			case 'S': // Snake (1 - 3)
				randInt = levelRoll2(randomEngine) + 1;
				ss << "lvl " << randInt << " Snake";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 3*randInt, 5*randInt, 10*randInt, 5*randInt, 2));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'g': // Goblin (2 - 4)
				randInt = levelRoll2(randomEngine) + 2;
				ss << "lvl " << randInt << " Goblin";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 6 * randInt, 4 * randInt, 15 * randInt, 10 * randInt, 5));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'B': // Bandit (3 - 5)
				randInt = levelRoll2(randomEngine) + 3;
				ss << "lvl " << randInt << " Bandit";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 5 * randInt, 5 * randInt, 20 * randInt, 15 * randInt, 25));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'O': // Ogre (4 - 6)
				randInt = levelRoll2(randomEngine) + 4;
				ss << "lvl " << randInt << " Ogre";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 7 * randInt, 2 * randInt, 25 * randInt, 20 * randInt, 8));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'I': // Imp (5 - 8)
				randInt = levelRoll3(randomEngine) + 5;
				ss << "lvl " << randInt << " Imp";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 8 * randInt, 1 * randInt, 10 * randInt, 10 * randInt, 15));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'K': // Evil Knight (6 - 9)
				randInt = levelRoll3(randomEngine) + 6;
				ss << "lvl " << randInt << " Evil Knight";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 5 * randInt, 5 * randInt, 15 * randInt, 10 * randInt, 10));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'd': // Demon (7 - 10)
				randInt = levelRoll3(randomEngine) + 7;
				ss << "lvl " << randInt << " Demon";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 1 * randInt, 10 * randInt, 30 * randInt, 15 * randInt, 20));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'F': // Animated Fire (8 - 11)
				randInt = levelRoll3(randomEngine) + 8;
				ss << "lvl " << randInt << " Animated Fire";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 10 * randInt, int(randInt/2), 5 * randInt, 10 * randInt, 7));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'L': // Lawyer (10 - 15)
				randInt = levelRoll5(randomEngine) + 10;
				ss << "lvl " << randInt << " Lawyer";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 1 * randInt, 20 * randInt, 10 * randInt, 20 * randInt, 50));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'Z': // Zombie (12 - 17)
				randInt = levelRoll5(randomEngine) + 12;
				ss << "lvl " << randInt << " Zombie";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, 15 * randInt, int(randInt/2), int(randInt/2), 10 * randInt, 5));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'A': // Adamantine (15 - 20)
				randInt = levelRoll5(randomEngine) + 15;
				ss << "lvl " << randInt << " Adamantine";
				_enemies.push_back(Enemy(ss.str(), tile, randInt, int(randInt/4), 50 * randInt, 50 * randInt, 30 * randInt, 10));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'U': // The Unknown (20 - 30)
				ss << "lvl " << unknownRandInt << " Unknown";
				_enemies.push_back(Enemy(ss.str(), tile, unknownRandInt, unknownRoll(randomEngine), unknownRoll(randomEngine), unknownRoll(randomEngine), unknownRoll(randomEngine), 50));
				_enemies.back().setPosition(j, i);
				ss.str("");
				break;
			case 'D': // Dragon
				_enemies.push_back(Enemy("lvl 100 Dragon", tile, 100, 2000, 2000, 2000, 5000, 100));
				_enemies.back().setPosition(j, i);
				break;
			case 'o': // Health Potion
				randInt = 25 * (healthRoll(randomEngine) + 1);
				ss << "a Strength " << randInt << " Health Potion";
				_items.push_back(Item(ss.str(), tile, randInt, 4 * randInt, false, false));
				_items.back().setPosition(j, i);
				ss.str("");
				break;
			case 'G': //satchel of gold coins
				randInt = roll(randomEngine);
				ss << 2*randInt + 1 << " Gold Coins";
				_items.push_back(Item(ss.str(), tile, 2*randInt + 1, 2*randInt + 1, false, false));
				_items.back().setPosition(j, i);
				ss.str("");
				break;
			case 'b': //Extra bag for inventory
				_items.push_back(Item("a magic bag for carrying things", tile, 1, 250, false, false));
				_items.back().setPosition(j, i);
				break;
			case 's': //Scroll for experience points
				_items.push_back(Item("an ancient scroll", tile, 25 * (healthRoll(randomEngine) + 1), 200, false, false));
				_items.back().setPosition(j, i);
				break;
			case 'l': // Lantern for seeing further
				_items.push_back(Item("a magic lantern", tile, 1, 500, false, false));
				_items.back().setPosition(j, i);
				break;
			case 'n': //A nice note to read! 
				_items.push_back(Item("an interesting note?", tile, 0, 0, false, false));
				_items.back().setPosition(j, i);
				break;
			case 't': // Sword
				randInt = roll(randomEngine);
				ss << "a Strength " << int(randInt / 10) + 1 << " Sword";
				_items.push_back(Item(ss.str(), tile, int(randInt / 10) + 1, 40*(int(randInt / 10) + 1), true, false));
				_items.back().setPosition(j, i);
				ss.str("");
				break;
			case 'W': // Shield
				randInt = roll(randomEngine);
				ss << "a Strength " << int(randInt / 10) + 1 << " Shield";
				_items.push_back(Item(ss.str(), tile, int(randInt / 10) + 1, 30*(int(randInt / 10) + 1), true, false));
				_items.back().setPosition(j, i);
				ss.str("");
				break;
			case 'Y': // Armour
				randInt = roll(randomEngine);
				ss << "a Strength " << int(randInt / 5) + 1 << " Armour";
				_items.push_back(Item(ss.str(), tile, int(randInt / 5) + 1, 50*(int(randInt / 5) + 1), true, false));
				_items.back().setPosition(j, i);
				ss.str("");
				break;
			case '*': // Artifact
				randInt = artifactRoll(randomEngine);
				switch (randInt) {
				case 0:
					_items.push_back(Item("the Holy Grail", tile, 20, 500, true, false));
					_items.back().setPosition(j, i);
					break;
				case 1:
					_items.push_back(Item("the Ring of Youth", tile, 25, 650, true, false));
					_items.back().setPosition(j, i);
					break;
				case 2:
					_items.push_back(Item("a Really, Really Nice Hat", tile, 12, 100, true, false));
					_items.back().setPosition(j, i);
					break;
				case 3:
					_items.push_back(Item("a Friendly Rat", tile, 15, 5, true, false));
					_items.back().setPosition(j, i);
					break;
				case 4:
					_items.push_back(Item("the God Stone", tile, 22, 1000, true, false));
					_items.back().setPosition(j, i);
					break;
				default:
					_items.push_back(Item("an Angry Ghoul", tile, 17, 500, true, false));
					_items.back().setPosition(j, i);
					break;
				}
				break;
			default:
				break;
			}
		}

	}
}

void Level::print(Player player) {

	string border = "   ";
	string ifEquipped = "";
	stringstream ss;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	_windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	_windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	if (_windowHeight - _mapHeight < 6 || _windowWidth - _mapWidth < 39) {
		printf("\nMap too large to fit the screen size, please readjust\n");
		system("PAUSE");
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		_windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		_windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if (_windowHeight - _mapHeight < 6 || _windowWidth - _mapWidth < 39) {
			//MoveWindow(GetConsoleWindow(), 0, 0, _screenWidth, _screenHeight, TRUE);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, _screenWidth, _screenHeight, SWP_SHOWWINDOW);
			SetWindowPos(GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			_windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			_windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			if (_windowHeight - _mapHeight < 6 || _windowWidth - _mapWidth < 39) {
				printf("\nMap cannot be played\n");
				system("PAUSE");
				exit(1);
			}
		}

	}

	//window and map need at least difference of
	//height:6 and width:39

	int windowSpaceHeight = _windowHeight - _mapHeight - 6;
	int windowSpaceWidth = _windowWidth - _mapWidth - 39;

	for (int i = 0; i < int(windowSpaceWidth/2); i++)
		ss << " ";

	int heightBorder = int(windowSpaceHeight / 2);
	string widthBorder = ss.str();
	ss.str("");

	for (int i = 0; i < _mapWidth; i++)
		ss << " ";
	string emptyMap = ss.str();
	ss.str("");

	for (unsigned i = 0; i < _levelData.size(); i++) {
		for (unsigned j = 0; j < _levelData[i].size(); j++) {
			if (_ifVisible[i][j])
				_visibleLevelData[i][j] = _levelData[i][j];
			else
				_visibleLevelData[i][j] = ' ';
		}
	}

	int playerX, playerY;
	player.getPosition(playerX, playerY);
	int distance = 0;
	int dx = 0, adx = 0, dy = 0, ady = 0;
	player.getPosition(playerX, playerY);
	for (unsigned i = 0; i < _ifVisible.size(); i++) {
		for (unsigned j = 0; j < _ifVisible[i].size(); j++) {
			dx = j - playerX;
			dy = i - playerY;
			adx = abs(dx);
			ady = abs(dy);
			distance = adx + ady;
			//if (distance <= player.getSightRange())
			if (adx < player.getSightRange() && ady < player.getSightRange())
				_ifVisible[i][j] = true;
			else
				_ifVisible[i][j] = false;
		}
	}

	//cout << string(45, '\n');
	system("CLS");
	
	string atkimp;
	string defimp;
	string levimp;
	string hpimp;

	if (_levelImp != 0) {
		ss << "   (+" << _levelImp << ")";
		levimp = ss.str();
		ss.str("");

		ss << "   (+" << _healthImp << ")";
		hpimp = ss.str();
		ss.str("");

		ss << "   (+" << _attackImp << ")";
		atkimp = ss.str();
		ss.str("");

		ss << "   (+" << _defenseImp << ")";
		defimp = ss.str();
		ss.str("");
	}

	string enemyHealth1, enemyHealth2, enemyHealth3, enemyHealth4;
	for (unsigned i = 0; i < _enemies.size(); i++) {
		if (_enemies[i].ifFighting()) {
			ss << _enemies[i].getName() << "'s health: " << _enemies[i].getHealth();
		}
		if (enemyHealth1 == "")
			enemyHealth1 = ss.str();
		else if (enemyHealth2 == "")
			enemyHealth2 = ss.str();
		else if (enemyHealth3 == "")
			enemyHealth3 = ss.str();
		else
			enemyHealth4 = ss.str();
		ss.str("");
	}

	for (int i = 0; i < heightBorder; i++)
		printf("\n");
	
	for (int i = 0; i <_mapHeight + windowSpaceHeight - heightBorder; i++) {
		printf("%s", widthBorder.c_str());
		if ((unsigned)i < _levelData.size())
			printf("%s", _visibleLevelData[i].c_str());
		else
			printf("%s", emptyMap.c_str());
		if (i == 0)
			printf("%sFloor %i", border.c_str(), _floor);
		if (i == 2)
			printf("%sLevel: %i %s", border.c_str(), player.getLevel(), levimp.c_str());
		if (i == 3)
			printf("%sHealth: %i/%i %s", border.c_str(), player.getHealth(), player.getMaxHealth(), hpimp.c_str());
		if (i == 4)
			printf("%sAttack: %i %s", border.c_str(), player.getAttack(), atkimp.c_str());
		if (i == 5)
			printf("%sExperience: %i/%i", border.c_str(), player.getExperience(), player.getExperienceLeft());
		if (i == 6)
			printf("%sDefense: %i %s", border.c_str(), player.getDefense(), defimp.c_str());
		if (i == 7)
			printf("%sGold: %i", border.c_str(), player.getGold());
		/*if (i == 9)
			printf("%sS = snake, g = Goblin, O = Ogre", border.c_str());
		if (i == 10)
			printf("%sD = Dragon, E = Exit, @ = You!", border.c_str());
		if (i == 11)
			printf("%s. = empty space, # = wall", border.c_str());
		if (i == 12)
			printf("%so = health potion", border.c_str());
		if (i == 13)
			printf("%sG = satchel of gold coins", border.c_str());*/
		if (i >= 9) {
			if (i < 9 + player.getInventorySize()) {
				if (player.getInventoryItem(i - 9).getIfEquipable()) {
					if (player.getInventoryItem(i - 9).getIfEquipped())
						ifEquipped = "(ON)";
					else
						ifEquipped = "(OFF)";
				}
				else
					ifEquipped = "";
				printf("%s%i: %s %s", border.c_str(), i - 8, player.getInventoryItem(i - 9).getName().c_str(), ifEquipped.c_str());
			}
			if (i == 10 + player.getInventorySize())
				printf("%s%s", border.c_str(), enemyHealth1.c_str());
			if (i == 11 + player.getInventorySize())
				printf("%s%s", border.c_str(), enemyHealth2.c_str());
			if (i == 12 + player.getInventorySize())
				printf("%s%s", border.c_str(), enemyHealth3.c_str());
			if (i == 13 + player.getInventorySize())
				printf("%s%s", border.c_str(), enemyHealth4.c_str());
		}
			
		printf("\n");
	}
	//Then do the spaces thing. I'm guessing 8 spaces between 
	if (_gameMessages.size() <= (unsigned int)(5)) {
		for (unsigned i = 0; i < 5 - _gameMessages.size(); i++)
			printf("\n");
	}
	for (unsigned i = 0; i < _gameMessages.size(); i++) {
		printf("%s\n", _gameMessages[i].c_str());
	//	cout << _gameMessages[i] << endl;
	}

	//printf("\n");

	//Now have the messages go here...

	//Then get rid of the messages
	_gameMessages.clear();
}


void Level::movePlayer(char input, Player &player, bool &ifUse, bool &ifDrop, bool &isDone) {

	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);

	char leaveTile = _currentStandingTile;
	
	if (!ifUse) {

		switch (input) {
		case 'w': // up
		case 'W':
			processPlayerMove(player, playerX, playerY - 1, isDone, leaveTile);
			break;
		case 's': // down
		case 'S':
			processPlayerMove(player, playerX, playerY + 1, isDone, leaveTile);
			break;
		case 'a': // left
		case 'A':
			processPlayerMove(player, playerX - 1, playerY, isDone, leaveTile);
			break;
		case 'd': // right
		case 'D':
			processPlayerMove(player, playerX + 1, playerY, isDone, leaveTile);
			break;
		case 'u': // use item
		case 'U':
			ifUse = true;
			ifDrop = false;
			break;
		case 't':
		case 'T':
			ifUse = true;
			ifDrop = true;
			break;
		default:
			printf("INVALID INPUT\n");
			system("PAUSE");
		}
	}
	else {
		int index = int(input) - 1;
		if (index < player.getInventorySize() && index >= 0) {
			if (!ifDrop) {
				if (player.getInventoryItem(index).getTile() != 'n') {
					if (player.getInventoryItem(index).getTile() == 's')
						player.addExperience(player.getInventoryItem(index).getValue(), _healthImp, _levelImp, _attackImp, _defenseImp);
					addGameMessage(player.useItem(index));
				}
				else
					readNote();
			}
			else {
				Item item;
				addGameMessage(player.dropItem(index, _currentStandingTile, item));
				_items.push_back(item);
				_items.back().setPosition(playerX, playerY);
			}
		}
		ifUse = false;
	}
}

void Level::updateEnemies(Player &player) {
	char aiMove;
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	player.getPosition(playerX, playerY);

	for (unsigned i = 0; i < _enemies.size(); i++) {
		aiMove = _enemies[i].getMove(playerX, playerY);
		_enemies[i].getPosition(enemyX, enemyY);

		switch (aiMove) {
		case 'w': // up
			processEnemyMove(player, i, enemyX, enemyY - 1);
			break;
		case 's': // down
			processEnemyMove(player, i, enemyX, enemyY + 1);
			break;
		case 'a': // left
			processEnemyMove(player, i, enemyX - 1, enemyY);
			break;
		case 'd': // right
			processEnemyMove(player, i, enemyX + 1, enemyY);
			break;
		}
	}
}

char Level::getTile(int x, int y) {
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile) {
	_levelData[y][x] = tile;
}


void Level::processPlayerMove(Player &player, int targetX, int targetY, bool &isDone, char &leaveTile) {
	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);
	//The tile you leave behind when you move, of course.

	char moveTile = getTile(targetX, targetY);

	switch (moveTile) {
	case '.':
		_currentStandingTile = '.';
		player.setPosition(targetX, targetY);
		setTile(playerX, playerY, leaveTile);
		setTile(targetX, targetY, '@');
		break;
	case '#':
		break;
	case 'E':
		isDone = true;
		if(_floor == 1)
			addGameMessage("You finished the 1st floor and entered the 2nd floor!!");
		if (_floor == 2)
			addGameMessage("You went through the 2nd floor and arrived at the 3rd!");
		if (_floor == 3)
			addGameMessage("The 3rd floor was no match for you! Onto the 4th!");
		if (_floor == 4)
			addGameMessage("Wow! That last floor was hard! To the 5th!");
		if (_floor == 5)
			addGameMessage("This is just embarrasing for me as a game developer... 6th floor");
		if (_floor == 6)
			addGameMessage("What?? Are you serious? I tried to make that level actually impossible");
		if (_floor == 7)
			addGameMessage("...8th now");
		if (_floor == 8)
			addGameMessage("...........");
		setTile(playerX, playerY, leaveTile);
		setTile(targetX, targetY, '@');
		break;
	case 'o':
	case 'G':
	case 'b':
	case 'W':
	case 't':
	case '*':
	case 'Y':
	case 's':
	case 'l':
	case 'n':
		addItems(player, targetX, targetY, leaveTile);
		break;
	default:
		battleMonster(player, targetX, targetY, true);
		break;
	}

	//playerX, playerY;
	player.getPosition(playerX, playerY);
	int distance = 0;
	int dx = 0, adx = 0, dy = 0, ady = 0;
	player.getPosition(playerX, playerY);
	for (unsigned i = 0; i < _ifVisible.size(); i++) {
		for (unsigned j = 0; j < _ifVisible[i].size(); j++) {
			dx = j - playerX;
			dy = i - playerY;
			adx = abs(dx);
			ady = abs(dy);
			distance = adx + ady;
			if (distance <= player.getSightRange())
				_ifVisible[i][j] = true;
		}
	}
	
}


void Level::processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY) {
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	_enemies[enemyIndex].getPosition(enemyX, enemyY);
	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);

	switch (moveTile) {
	case '.':
		_enemies[enemyIndex].setPosition(targetX, targetY);
		setTile(enemyX, enemyY, '.');
		setTile(targetX, targetY, _enemies[enemyIndex].getTile());
		break;
	case '@':
		battleMonster(player, enemyX, enemyY, false);
		break;
	default:
		break;
	}
}

void Level::battleMonster(Player &player, int targetX, int targetY, bool ifPlayer) {

	int enemyX;
	int enemyY;
	int playerX;
	int playerY;
	int attackRoll;
	int attackResult;
	string enemyName;
	stringstream ss;

	player.getPosition(playerX, playerY);

	for (unsigned i = 0; i < _enemies.size(); i++) {

		_enemies[i].getPosition(enemyX, enemyY);
		enemyName = _enemies[i].getName();

		if (targetX == enemyX && targetY == enemyY) {

			if (ifPlayer) {
				//battle!
				attackRoll = player.attack();
				attackResult = _enemies[i].takeDamage(attackRoll, player.getAttack());
				//printf("\nPlayer attacked %s with a roll of %d\n", enemyName.c_str(), attackRoll);
				ss << "Player inflictied " << attackRoll << " damage on " << enemyName << "!";
				addGameMessage(ss.str());
				ss.str("");
				if (attackResult != 0) {
					Item drop = _enemies[i].chooseDropItem();
					if (drop.getTile() != '.') {
						_items.push_back(drop);
						_items.back().setPosition(targetX, targetY);
					}
					setTile(targetX, targetY, drop.getTile());
					//print(player);
					//printf("Monster died!\n");
					ss << "Monster died!";
					addGameMessage(ss.str());
					ss.str("");
					ss << "You earned " << int(attackResult * player.getExperienceMultiplier()) << " experience points!";
					addGameMessage(ss.str());
					ss.str("");
					_enemies[i] = _enemies.back();
					_enemies.pop_back();
					i--;

					//system("PAUSE");
					string s = player.addExperience(attackResult, _healthImp, _levelImp, _attackImp, _defenseImp);
					if (s != "")
						addGameMessage(s);
					//setTile(targetX, targetY, '.');
					return;
				}
			}
			else {
				//Monster fight
				attackRoll = _enemies[i].attack();
				attackResult = player.takeDamage(attackRoll, _enemies[i].getAttack());
				//printf("%s attacked player with a roll of %d\n", enemyName.c_str(), attackRoll);
				ss << enemyName << " attacked player, inflicting " << attackRoll << " damage!";
				addGameMessage(ss.str());
				ss.str("");

				if (attackResult != 0) {
					setTile(playerX, playerY, 'x');
					//print(player);
					//printf("You died!\n");
					ss << "You died!";
					addGameMessage(ss.str());
					ss.str("");
					print(player);
					system("PAUSE");
					//Handle Death by going back to main menu? Or back to beginning of floor using temporarySave.txt?
					_ifDead = true;
					ss << "You died from a particularly powerful " << enemyName << " who killed you savagely...";
					_deathMessage = ss.str();
					ss.str("");
					//exit(0);
				}
			}

			//system("PAUSE");
			return;
		}
	}
}

void Level::addItems(Player &player, int targetX, int targetY, char leaveTile) {
	int itemX;
	int itemY;
	int playerX;
	int playerY;
	stringstream ss;

	player.getPosition(playerX, playerY);
	
	for (unsigned i = 0; i < _items.size(); i++) {

		_items[i].getPosition(itemX, itemY);
		string itemName = _items[i].getName();

		if (targetX == itemX && targetY == itemY) {

			if (player.getActualInventorySpace() > 0 && _items[i].getTile() != 'G') {

				ss << "You picked up " << _items[i].getName();
				addGameMessage(ss.str());
				ss.str("");

				player.setPosition(targetX, targetY);
				setTile(playerX, playerY, leaveTile);
				setTile(targetX, targetY, '@');


				player.addToInventory(_items[i]);
				_currentStandingTile = '.';

				_items[i] = _items.back();
				_items.pop_back();
				i--;
				
			}
			else if (_items[i].getTile() == 'G') {

				ss << "You picked up " << _items[i].getName();
				addGameMessage(ss.str());
				ss.str("");

				player.addGold(_items[i].getValue());

				_currentStandingTile = '.';

				player.setPosition(targetX, targetY);
				setTile(playerX, playerY, leaveTile);
				setTile(targetX, targetY, '@');
			}
			else {
				ss << "You see " << _items[i].getName() << " but can't pick it up because your inventory is full";
				addGameMessage(ss.str());
				ss.str("");

				player.setPosition(targetX, targetY);
				setTile(playerX, playerY, leaveTile);
				setTile(targetX, targetY, '@');

				_currentStandingTile = _items[i].getTile();
			}
		}
	}
	//playerX, playerY;
	player.getPosition(playerX, playerY);
	int distance = 0;
	int dx = 0, adx = 0, dy = 0, ady = 0;
	player.getPosition(playerX, playerY);
	for (unsigned i = 0; i < _ifVisible.size(); i++) {
		for (unsigned j = 0; j < _ifVisible[i].size(); j++) {
			dx = j - playerX;
			dy = i - playerY;
			adx = abs(dx);
			ady = abs(dy);
			distance = adx + ady;
			if (distance <= player.getSightRange())
				_ifVisible[i][j] = true;
		}
	}
}


void Level::readNote() {

	string widthBorder = "";
	system("CLS");

	vector <string> noteMessage;

	ifstream file;

	file >> std::noskipws;

	file.open(_currentNoteFileName);

	if (file.fail()) {
		perror(_currentNoteFileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;
	int width = 0;
	int height = 0;
	while (getline(file, line)) {
		height += 1;
		if (line.size() > width) width = line.size();
		noteMessage.push_back(line);
	}

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
	for (int i = 0; i < int(widthSpace / 2); i++)
		widthBorder += " ";
	for (int i = 0; i < int(heightSpace / 2); i++)
		printf("\n");

	for (int i = 0; i < noteMessage.size(); i++)
		printf("%s%s\n", widthBorder.c_str(), noteMessage[i].c_str());
	
	for (int i = 0; i < heightSpace - int(heightSpace / 2); i++)
		printf("\n");

	system("PAUSE");
}