#pragma once
#include "Player.h"
#include "Level.h"
#include<string>

class GameSystem
{
public:
	GameSystem();

	void newFloor(string levelFileName, string noteFileName);
	void restart(string levelFileName, string noteFileName, string campaign, string playerName);
	void loadGame(string saveFileName, string &campaign, int &currentFloor);

	void playGame();
	void playerMove(bool &ifUse, bool &ifDrop, bool &isDone);
	void printInfo();
	void shopScreen();
	void continueScreen();
	void saveScreen();
	void saveGame(string saveFileName);
	void deathScreen(string deathMessage);

	bool getIfDeath() { return _ifDeath; }

private:
	Player _player;
	Level _level;
	int _currentFloor;
	string _campaign;

	vector <string> _saveFileNames;

	bool _ifDeath;
};

