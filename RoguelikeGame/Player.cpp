#include "Player.h"
#include "Level.h"
#include<string>
#include<sstream>
#include <random>
#include <ctime>

using namespace std;

Player::Player()
{
	for (int i = 0; i < 4; i++)
		_equipment[i] = Item();
	_x = 0;
	_y = 0;
}


void Player::init(string name, int level, int health, int attack, int defense, int experience, int experienceLeft, double experienceMultiplier, int maxHealth, int gold, int inventorySpace, int sightRange) {
	_name = name;
	_level = level;
	_health = health;
	_attack = attack;
	_defense = defense;
	_experience = experience;
	_experienceLeft = experienceLeft;
	_maxHealth = maxHealth;
	_gold = gold;
	_sightRange = sightRange;
	_inventorySpace = inventorySpace;
	_experienceMultiplier = experienceMultiplier;
}

int Player::attack() {
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> attackRoll(0, _attack);

	return attackRoll(randomEngine);
}


string Player::useItem(int index) {
	string gameMessage = "";
	stringstream ss;
	int slot = 0;
	bool ifError = false;

	switch (_inventory[index].getTile()) {
	case 'o':
		_health += _inventory[index].getValue();
		if (_health > _maxHealth)
			_health = _maxHealth;
		ss << "You used a health potion and recovered " << _inventory[index].getValue() << " health!!";
		gameMessage = ss.str();
		ss.str("");
		break;
	case 'b':
		if (_inventorySpace < _maxInventorySpace) {
			_inventorySpace += 1;
			ss << "The magic bag vanishes and allows you to carry one more item!";
		}
		else
			ss << "The magic bag vanishes... but does nothing";
		gameMessage = ss.str();
		break;
	case 's':
		ss << "You read the scroll and gained " << _inventory[index].getValue() << " experience!!";
		gameMessage = ss.str();
		ss.str("");
		break;
	case 'l':
		_sightRange += 1;
		ss << "The lantern shatters, and suddenly the room doesn't seem as dark...?";
		gameMessage = ss.str();
		ss.str("");
		break;
	case 't':
		slot = 0;
		if (!_inventory[index].getIfEquipped() && "none" == _equipment[slot].getName()) {
			_attack += _inventory[index].getValue();
			ss << "You equipped the sword and raised your attack by " << _inventory[index].getValue();
		}
		else if (_inventory[index].getID() == _equipment[slot].getID()) {
			_attack -= _inventory[index].getValue();
			ss << "You put away the sword and your attack dropped by " << _inventory[index].getValue();
		}
		else {
			ss << "You can't equip more than one sword!!!";
			ifError = true;
		}
		gameMessage = ss.str();
		break;
	case 'W':
		slot = 1;
		if (!_inventory[index].getIfEquipped() && "none" == _equipment[slot].getName()) {
			_defense += _inventory[index].getValue();
			ss << "You equipped the shield and raised your defense by " << _inventory[index].getValue();
		}
		else if (_inventory[index].getID() == _equipment[slot].getID()) {
			_defense -= _inventory[index].getValue();
			ss << "You put away the shield and your defense dropped by " << _inventory[index].getValue();
		}
		else {
			ss << "You can't equip more than one shield!!!";
			ifError = true;
		}
		gameMessage = ss.str();
		break;
	case 'Y':
		slot = 2;
		if (!_inventory[index].getIfEquipped() && "none" == _equipment[slot].getName()) {
			_maxHealth += 2 * _inventory[index].getValue();
			ss << "You equipped the armour and raised your max health by " << 2 * _inventory[index].getValue();
		}
		else if (_inventory[index].getID() == _equipment[slot].getID()) {
			_maxHealth -= 2 *_inventory[index].getValue();
			ss << "You put away the armour and your max health dropped by " << 2 * _inventory[index].getValue();
		}
		else {
			ss << "You can't equip more than one set of armour!!!";
			ifError = true;
		}
		gameMessage = ss.str();
		break;
	case '*':
		slot = 3;
		if (!_inventory[index].getIfEquipped() && "none" == _equipment[slot].getName()) {
			// Make either experience or something
			_experienceMultiplier = float(_inventory[index].getValue())/10.0f;
			ss << "You equipped " << _inventory[index].getName() << " and something mysterious happened :)";
			gameMessage = ss.str();
		}
		else if (_inventory[index].getID() == _equipment[slot].getID()) {
			ss << "You put away " << _inventory[index].getName() << " and something mysterious stopped happening...";
			gameMessage = ss.str();
			_experienceMultiplier = 1.0;
		}
		else {
			ss << "You can't equip more than one artifact!!!";
			ifError = true;
		}
		break;
	default:
		gameMessage = "ERROR";
		ifError = true;
		break;
	}
	if (!ifError) {
		if (_inventory[index].getIfEquipable()) {
			if (!_inventory[index].getIfEquipped())
				_equipment[slot] = _inventory[index];
			else
				_equipment[slot] = Item();

			_inventory[index].equipToggle();
		}
		else {
			_inventory[index] = _inventory.back();
			_inventory.pop_back();
		}
	}

	return gameMessage;
}

string Player::dropItem(int index, char &leaveTile, Item &item) {
	stringstream ss;

	if (_inventory[index].getIfEquipped())
		return "You can't throw away something you're wearing!!";
	else if (leaveTile != '.') {
		return "There's already something underneath you!";
	} 
	else {
		ss << "You threw away " << _inventory[index].getName() << " for some reason...";
		leaveTile = _inventory[index].getTile();
		item = _inventory[index];
		_inventory[index] = _inventory.back();
		_inventory.pop_back();
		return ss.str();
	}
}

//Setters
void Player::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

void Player::addToInventory(Item item) {
	_inventory.push_back(item);
}

void Player::equip(Item item) {
	int i = 0;
	switch(item.getTile()) {
	case 't':
		i = 0;
		break;
	case 'W':
		i = 1;
		break;
	case 'Y':
		i = 2;
		break;
	case '*':
		i = 3;
		break;
	default:
		return;
		break;
	}
	_equipment[i] = item;
}


string Player::addExperience(int experience, int &healthImp, int &levelImp, int &attackImp, int &defenseImp) {
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> levelRoll(1, 10);
	uniform_int_distribution<int> healthRoll(1, 30);

	int randInt = 0;

	_experience += int(_experienceMultiplier * experience);
	int levelsAdvanced = 0;
	stringstream ss;
	int maxHealthB4 = _maxHealth;
	int levelB4 = _level;
	int attackB4 = _attack;
	int defenseB4 = _defense;

	while (_experience >= _experienceLeft) {
		_experience -= _experienceLeft;
		_experienceLeft += 25;
		levelsAdvanced += 1;
		randInt = levelRoll(randomEngine);
		_attack += randInt;
		randInt = levelRoll(randomEngine);
		_defense += randInt;
		randInt = healthRoll(randomEngine);
		_maxHealth += randInt;
		_health = _maxHealth;
		_level++;
	}

	attackImp = _attack - attackB4;
	defenseImp = _defense - defenseB4;
	healthImp = _maxHealth - maxHealthB4;
	levelImp = _level - levelB4;

	if (levelsAdvanced == 1)
		ss << "Nice job! You leveled up!";
	else if (levelsAdvanced > 1)
		ss << "Wow! You leveled up " << levelsAdvanced << " times!!";
	else {
		ss << "";
		attackImp = 0; healthImp = 0;
		defenseImp = 0; levelImp = 0;
	}

	return ss.str();
}

//Getters
void Player::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}

int Player::takeDamage(int &attack, int attackStat) {
	attack = int(attack + attackStat/_defense) + 1;
	//check if attack does damage
	if (attack > 0) {
		_health -= attack;
		//check if he died
		if (_health <= 0) {
			_health = 0;
			return 1;
		}
	}
	else attack = 0;
	return 0;
}

string Player::buyItem(Item item) {
	if (item.getSellValue() <= _gold) {
		if (getActualInventorySpace() > 0) {
			_inventory.push_back(item);
			_gold -= item.getSellValue();
			return "";
		}
		else return "You don't have enough space for that!! ";
	}
	else
		return "You don't have enough money to buy that!! ";
}

string Player::sellItem(int index) {

	if (index >= 0 && index < getInventorySize()) {
		_gold += int(0.75*_inventory[index].getSellValue());
		for (int i = 0; i < 4; i++) {
			if (_equipment[i].getID() == _inventory[index].getID())
				_equipment[i] = Item();
		}
		_inventory[index] = _inventory.back();
		_inventory.pop_back();
		return "";
	}
	else
		return "Stop yanking my chain like that!!! ";
}