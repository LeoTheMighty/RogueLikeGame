#include "Item.h"
#include <random>
#include <ctime>


Item::Item()
{
	_name = "none";
	_tile = '.';
	_value = 0;
	_sellValue = 0;
	_ifEquipable = false;
	_ifEquipped = false;
	//static default_random_engine randomEngine((unsigned int)time(NULL));
	//uniform_int_distribution<int> roll(0, 100000);
	//_id = roll(randomEngine);
	_id = 0;

}

Item::Item(string name, char tile, int value, int sellValue, bool ifEquipable, bool ifEquipped)
{
	_name = name;
	_tile = tile;
	_value = value;
	_sellValue = sellValue;
	_ifEquipable = ifEquipable;
	_ifEquipped = ifEquipped;
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> roll(0, 100000);
	_id = roll(randomEngine);
	
}

//Setters
void Item::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

//Getters
void Item::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}

