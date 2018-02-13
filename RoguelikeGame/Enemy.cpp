#include "Enemy.h"
#include <random>
#include <iostream>
#include <ctime>
#include <sstream>
#include<string>
#include "Player.h"

using namespace std;

Enemy::Enemy(string name, char tile, int level, int attack, int defense, int health, int xp, int range)
{
	_name = name;
	_level = level;
	_tile = tile;
	_attack = attack;
	_defense = defense;
	_health = health;
	_experienceValue = xp;
	_attackRange = range;
	_ifFighting = false;

	stringstream ss;
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> roll(0, 50 * level);
	uniform_int_distribution<int> healthRoll(0, level);
	uniform_int_distribution<int> artifactRoll(0, 5);
	int randInt;

	//_possibleDrops = { hp potion, gold, bag, sword, shield, armour, artifact }
	_possibleDrops.push_back(Item());
	randInt = 25 * (healthRoll(randomEngine) + 1);
	ss << "a Strength " << randInt << " Health Potion";
	_possibleDrops.push_back(Item(ss.str(), 'o', randInt, 4 * randInt, false, false));
	ss.str("");
	randInt = roll(randomEngine);
	ss << 3*randInt << " Gold Coins";
	_possibleDrops.push_back(Item(ss.str(), 'G', 3*randInt, 3*randInt, false, false));
	ss.str("");
	_possibleDrops.push_back(Item("a magic bag for carrying things", 'b', 1, 250, false, false));
	_possibleDrops.push_back(Item("an ancient scroll", 's', 25 * (healthRoll(randomEngine) + 1), 200, false, false));
	_possibleDrops.push_back(Item("a magic lantern", 'l', 1, 500, false, false));
	randInt = roll(randomEngine);
	ss << "a Strength " << int(randInt / 10) + 1 << " Sword";
	_possibleDrops.push_back(Item(ss.str(), 't', int(randInt / 10) + 1, 4 * randInt, true, false));
	ss.str("");
	randInt = roll(randomEngine);
	ss << "a Strength " << int(randInt / 10) + 1 << " Shield";
	_possibleDrops.push_back(Item(ss.str(), 'W', int(randInt / 10) + 1, 3 * randInt, true, false));
	ss.str("");
	randInt = roll(randomEngine);
	ss << "a Strength " << int(randInt / 5) + 1 << " Armour";
	_possibleDrops.push_back(Item(ss.str(), 'Y', int(randInt / 5) + 1, 10 * randInt, true, false));
	ss.str("");
	randInt = artifactRoll(randomEngine);
	switch (randInt) {
	case 0:
			_possibleDrops.push_back(Item("the Holy Grail", '*', 20, 500, true, false));
			break;
		case 1:
			_possibleDrops.push_back(Item("the Ring of Youth", '*', 25, 650, true, false));
			break;
		case 2:
			_possibleDrops.push_back(Item("a Really, Really Nice Hat", '*', 12, 100, true, false));
			break;
		case 3:
			_possibleDrops.push_back(Item("a Friendly Rat", '*', 15, 5, true, false));
			break;
		case 4:
			_possibleDrops.push_back(Item("the God Stone", '*', 22, 1000, true, false));
			break;
		default:
			_possibleDrops.push_back(Item("an Angry Ghoul", '*', 50, 500, true, false));
			break;
	}


	//_possibleDrops = { none, hp potion, gold, bag, scroll, lantern, sword, shield, armour, artifact }

	_percentSum = 0;
	_percentChance = { 1000 - (20 * level), 75 + level, 60 + level, 40 + 2 * level, 30 + 2 * level, 20 + int(2.5*level) , 15 + 3 * level, 15 + 3 * level, 5 + 4 * level, 5 * level };
	for (unsigned i = 0; i < _percentChance.size(); i++)
		_percentSum += _percentChance[i];

	/*
	switch (level) {
	case 1:
		_percentChance = { 75, 15, 10, 0, 0, 0, 0, 0, 0 };
		break;
	case 2:
		_percentChance = { 75, 10, 10, 5, 0, 0, 0, 0, 0 };
		break;
	case 3:
		_percentChance = { 30, 15, 10, 5, 1, 1, 1, 1, 1 };
		break;
	case 4:
		_percentChance = { 30, 15, 15, 15, 5, 5, 5, 5, 5 };
		break;
	case 5:
		_percentChance = { 25, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 6:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 7:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 9:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 10:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 11:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 12:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 13:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 14:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 15:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 16:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 17:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 18:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 19:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	case 20:
		_percentChance = { 30, 15, 15, 20, 5, 5, 5, 5, 5 };
		break;
	default: 
		_percentChance = { 10, 10, 10, 10, 15, 15, 15, 15 };
		break;
	} */
}

int Enemy::attack() {
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> attackRoll(0, _attack);

	return attackRoll(randomEngine);
}

Item Enemy::chooseDropItem() {
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> roll(0, _percentSum);
	int percentInt = 0;
	int randInt = roll(randomEngine);
	Item item = Item();

	for (unsigned i = 0; i < _possibleDrops.size(); i++) {
		percentInt += _percentChance[i];
		if (randInt <= percentInt) {
			item = _possibleDrops[i];
			_possibleDrops.clear();
			_percentChance.clear();
			break;
		}
	}
	return item;
	
}

int Enemy::takeDamage(int &attack, int attackStat) {
	attack = int(attack + attackStat/_defense) + 1;
	//check if attack does damage
	if (attack > 0) {
		_health -= attack;
		//check if he died
		if (_health <= 0) {
			return _experienceValue;
		}
	}
	else attack = 0;
	return 0;
}


char Enemy::getMove(int playerX, int playerY) {

	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> moveRoll(0, 6);

	int distance;
	int dx = _x - playerX;
	int dy = _y - playerY;
	int adx = abs(dx);
	int ady = abs(dy);

	distance = adx + ady;

	if (distance <= 10) {
		fight();
	}
	else
		unfight();

	if (distance <= _attackRange) {
		//moving along x axis
		if (adx > ady) {
			if (dx > 0) {
				return 'a';
			}
			else {
				return 'd';
			}
		}
		// move along y axis
		else {
			if (dy > 0) {
				return 'w';
			}
			else {
				return 's';
			}
		}
	}

	int randomMove = moveRoll(randomEngine);
	switch (randomMove) {
	case 0:
		return 'a';
	case 1:
		return 'w';
	case 2:
		return 's';
	case 3:
		return 'd';
	default:
		return '.';
	}

}


//Setters
void Enemy::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

//Getters
void Enemy::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}
