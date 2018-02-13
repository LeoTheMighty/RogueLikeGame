#pragma once
#include<string>

using namespace std;

class Item
{
public:
	Item();
	Item(string name, char tile, int value, int sellValue, bool ifEquipable, bool ifEquipped);

	//Setters
	void setPosition(int x, int y);
	void equipToggle() { if (_ifEquipped) _ifEquipped = false; else _ifEquipped = true; }

	//Getters
	void getPosition(int &x, int &y);
	string getName() { return _name; }
	char getTile() { return _tile; }
	int getValue() { return _value; }
	bool getIfEquipped() { return _ifEquipped; }
	bool getIfEquipable() { return _ifEquipable; }
	int getID() { return _id; }
	int getSellValue() { return _sellValue; }

private:
	string _name;
	char _tile;
	int _id;

	bool _ifEquipable;
	bool _ifEquipped;
	int _value;
	int _sellValue;

	//Items can....?
	//A. give you experience
	//B. give you health
	//C. give you attack
	//D. give you defense
	//E. read a funny note

	//position
	int _x;
	int _y;
};

