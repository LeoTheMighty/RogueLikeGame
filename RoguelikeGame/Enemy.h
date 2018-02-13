#pragma once
#include<string>
#include<vector>
#include "Item.h"
using namespace std;

class Enemy
{
public:
	Enemy(string name, char tile, int level, int attack, int defense, int health, int xp, int range);

	int attack();
	int takeDamage(int &attack, int attackStat);

	Item chooseDropItem();

	//Gets AI move command
	char getMove(int playerX, int playerY);

	//Setters
	void setPosition(int x, int y);
	void fight() { _ifFighting = true; }
	void unfight() { _ifFighting = false; }

	//Getters
	void getPosition(int &x, int &y);
	string getName() { return _name; }
	char getTile() { return _tile; }
	int getRange() { return _attackRange; }
	int getAttack() { return _attack; }
	int getHealth() { return _health; }
	bool ifFighting() { return _ifFighting; }

private:
	string _name;
	char _tile;
	bool _ifFighting;

	int _level;
	int _attack;
	int _defense;
	int _health;
	int _experienceValue;
	int _attackRange;

	int _percentSum;

	//Position
	int _x;
	int _y;

	vector<Item> _possibleDrops;
	vector<int> _percentChance;

};