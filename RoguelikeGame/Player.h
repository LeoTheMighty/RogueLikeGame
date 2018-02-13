#pragma once
#include "Level.h"
#include "Enemy.h"
#include "Item.h"
#include<string>

class Level;

class Player
{
public:
	//in save file, you need current level, health, attack, defense, experience, experienceLeft
	//maxHealth, gold, sightRange, equipment, inventory, experienceMultiplier, campaign, floor
	Player();
	void init(string name, int level, int health, int attack, int defense, int experience, int experienceLeft, double experienceMultiplier, int maxHealth, int gold, int inventorySpace, int sightRange);

	int attack();

	string useItem(int index);
	string dropItem(int index, char &leaveTile, Item &item);

	string buyItem(Item item);
	string sellItem(int index);

	//Setters
	void setPosition(int x, int y);
	void addToInventory(Item item);
	//only to be used when loading saved game
	void equip(Item item);
	void addGold(int gold) { _gold += gold; }

	void clearEquipment() { for (int i = 0; i < 4; i++) _equipment[i] = Item(); }
	void clearInventory() { _inventory.clear(); }

	std::string addExperience(int experience, int &healthImp, int &levelImp, int &attackImp, int &defenseImp);

	//Getters
	void getPosition(int &x, int &y);
	string getName() { return _name; }
	int getLevel() { return _level; }
	int getHealth() { return _health; }
	int getAttack() { return _attack; }
	int getDefense() { return _defense; }
	int getExperience() { return _experience; }
	int getExperienceLeft() { return _experienceLeft; }
	double getExperienceMultiplier() { return _experienceMultiplier; }
	int getMaxHealth() { return _maxHealth; }
	int getGold() { return _gold; }
	int getSightRange() { return _sightRange; }

	int getActualInventorySpace() { return _inventorySpace - _inventory.size(); }
	int getInventorySpace() { return _inventorySpace; }
	int getInventorySize() { return _inventory.size(); }
	Item getInventoryItem(int index) { return _inventory[index]; }

	int takeDamage(int &attack, int attackStat);

private:
	//Properties
	string _name;
	int _level;
	int _health;
	int _attack;
	int _defense;
	int _experience;
	int _experienceLeft;
	int _maxHealth;

	int _sightRange;

	double _experienceMultiplier;

	int _inventorySpace;
	const int _maxInventorySpace = 9;

	int _gold;

	vector<Item> _inventory;

	//First slot is sword, second is shield, third is armour, fourth is artifact
	Item _equipment[4];
	//inventory as well?

	//Position
	int _x;
	int _y;
};

