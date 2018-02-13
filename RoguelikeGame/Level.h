#pragma once

#include<vector>
#include<string>

#include "Item.h"

#include "Player.h"
#include "Enemy.h"

using namespace std;

class Player;

class Level
{
public:
	Level();

	void load(string fileName, Player &player);
	void print(Player player);

	void movePlayer(char input, Player &player, bool &ifUse, bool &ifDrop, bool &isDone);
	void updateEnemies(Player &player);
	//Getters
	char getTile(int x, int y);
	void readNote();
	bool ifVisible(int x, int y) { return _ifVisible[y][x]; }
	string getIfDead(bool &ifDead) { ifDead = _ifDead; return _deathMessage; }
	void setIfDead(bool ifDead) { _ifDead = ifDead; }
	//Setters
	void setTile(int x, int y, char tile);
	void addGameMessage(string message) { _gameMessages.push_back(message); }
	void clearLevelData() { _levelData.clear(); }
	void clearEnemies() { _enemies.clear(); }
	void clearItems() { _items.clear(); }
	void clearIfVisible() { _ifVisible.clear(); }
	void clearVisibleLevelData() { _visibleLevelData.clear(); }
	void setNoteFileName(string fileName) { _currentNoteFileName = fileName; }
	void resetLevel() { _floor = 0; }

	
private:
	void processPlayerMove(Player &player, int targetX, int targetY, bool &isDone, char &leaveTile);
	void processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY);
	void battleMonster(Player &player, int targetX, int targetY, bool ifPlayer);
	void addItems(Player &player, int targetX, int targetY, char leaveTile);


	vector <string> _levelData;
	vector <vector<bool>> _ifVisible;
	vector <string> _visibleLevelData;
	vector <Enemy> _enemies;
	vector <string> _gameMessages;
	vector <Item> _items;

	int _mapHeight;
	int _mapWidth;
	int _windowWidth;
	int _windowHeight;
	int _screenHeight;
	int _screenWidth;
	int _floor;
	char _currentStandingTile;
	string _currentNoteFileName;

	bool _ifDead;
	string _deathMessage;

	//player improvements
	int _levelImp;
	int _attackImp;
	int _defenseImp;
	int _healthImp;
};

