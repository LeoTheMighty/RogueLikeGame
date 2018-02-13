#include<conio.h>
#include<string>
#include<cstdlib>
#include<sstream>
#include<iostream>
#include<fstream>
#include<random>
#include<ctime>
#include<Windows.h>

#include "GameSystem.h"

using namespace std;

void mainMenu(string &campaign, int &campaignLength, string &saveFileName, string &playerName, bool &ifNewGame, bool &ifLoadGame, bool &isPlaying);

int main()
{
	//New Game, Load Game, Choose Campaign, Exit.
	stringstream ss, ss1;
	string campaign = "";
	string saveFileName = "";
	string playerName = "";
	bool isPlaying = true;
	bool ifLoadGame = false;
	bool ifNewGame = false;
	//bool ifInitialized = false;
	int campaignLength = 9;
	int currentFloor = 1;

	//Main Menu
	//mainMenu(campaign, campaignLength, saveFileName, ifNewGame, ifLoadGame, isPlaying);

	//static default_random_engine randomEngine((unsigned int)time(NULL));
	//uniform_int_distribution<int> roll(1, 9);


	//int randInt = roll(randomEngine);
	//randInt = 1;

	//ofstream file("SaveFiles/new.txt");

	//file << "New line of text";

	//ss << "Levels/Level" << campaign << currentFloor << ".txt";
	//ss1 << "Notes/Note" << campaign << currentFloor << ".txt";

	//GameSystem gameSystem(ss.str(), ss1.str(), campaign);

	// Restart Main Menu
	//if(ifInitialized)
	//gameSystem.restart(ss.str(), ss1.str(), campaign);

	//ss.str("");
	//ss1.str("");

	//gameSystem.playGame();

	/*
	if you want a randomized floor system

	while(isPlaying) {
		randInt = roll(randomEngine);
		ss << "Levels/Level" << randInt << ".txt";
		gameSystem.newFloor(ss.str());
		ss.str("");
		gameSystem.playGame();
	} */

	GameSystem gameSystem;

	while (isPlaying) {
		ifNewGame = false;
		ifLoadGame = false;

		mainMenu(campaign, campaignLength, saveFileName, playerName, ifNewGame, ifLoadGame, isPlaying);

		ss << "Levels/Level" << campaign << currentFloor << ".txt";
		ss1 << "Notes/Note" << campaign << currentFloor << ".txt";

		if (ifNewGame)
			gameSystem.restart(ss.str(), ss1.str(), campaign, playerName);
		if (ifLoadGame)
			gameSystem.loadGame(saveFileName, campaign, currentFloor);

		ss.str("");
		ss1.str("");
		if(isPlaying)
			gameSystem.playGame();

		if (!gameSystem.getIfDeath() && isPlaying) {

			for (int i = 2; i <= campaignLength; i++) {
				ss << "Levels/Level" << campaign << i << ".txt";
				ss1 << "Notes/Note" << campaign << i << ".txt";

				gameSystem.newFloor(ss.str(), ss1.str());
				ss.str("");
				ss1.str("");
				//returns if back to menu because death or continues to next stage
				gameSystem.playGame();
				if (gameSystem.getIfDeath())
					i = campaignLength + 1;
				//if death then i = campaignLength + 1;
			}
		}

		//mainMenu();


		//JK This will go in GameSystem.cpp (
		// Maybe displays a death message really eerily ya know?
		// Also brings back to main menu? )

		//so it goes you go to main menu to initialize the actual game, and you can either load a game, start a new game, or quit 
		//(or maybe view top scorers as well? Probably in terms of level or whatever)
		//Then the init function should also take in the saveFileName and an ifLoadGame variable
		//Then it plays the game yadda yadda until you either quit or die, then it brings you back to the main menu, and 
		//the restart function will basically init another gameSystem, except it will use the game system and just clear out all the 
		//variables and replace it with the new ones.
	}

	//printf("Thanks for playing!!\n");
	system("PAUSE");
	return 0;
}

void mainMenu(string &campaign, int &campaignLength, string &saveFileName, string &playerName, bool &ifNewGame, bool &ifLoadGame, bool &isPlaying) {
	//stringstream ss;
	string response;
	bool a = true;
	int height = 15;
	int width = 74;
	int heightBorder = 3;
	string widthBorder;
	vector <string> campaigns;
	vector <int> campaignLengths;
	vector <int> campaignSpaceLengths;
	vector <string> saveFiles;
	vector <int> saveSpaceLengths;
	int screen = 1;
	// 1 is ROGUE screen, 2 is new game choosing campaign, 3 is new game choosing name, 3 is load game choosing saved games

	ifstream campaginFile;

	campaginFile.open("Campaigns.txt");

	if (campaginFile.fail()) {
		perror("Campaigns.txt");
		system("PAUSE");
		exit(1);
	}

	string tempCampaign;
	int tempLength;
	//while (getline(file, line)) {
	while (campaginFile >> tempCampaign >> tempLength) {
		//std::istringstream iss(line);
		//string a;
		//int b;
		//if (!(iss >> a >> b)) { break; } // error
		campaigns.push_back(tempCampaign);
		campaignLengths.push_back(tempLength);
	}

	campaginFile.close();

	ifstream saveFile;

	saveFile.open("SaveFileNames.txt");

	if (saveFile.fail()) {
		perror("Campaigns.txt");
		system("PAUSE");
		exit(1);
	}

	string line;

	while (saveFile >> line)
		saveFiles.push_back(line);

	saveFile.close();

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

	width = 74;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	height = 15;

	int widthSpace = windowWidth - width;
	int heightSpace = windowHeight - height - 1;

	//Probably use the same centering technique...
	while (a) {
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
		widthBorder = "";
		for (int i = 0; i < int(widthSpace / 2); i++)
			widthBorder += " ";
		for (int i = 0; i < int(heightSpace / 2); i++)
			printf("\n");

		if (screen == 1) {
			printf("%s                                                                    ____\n", widthBorder.c_str());
			printf("%s|--------\\                                                         |    |\n", widthBorder.c_str());
			printf("%s|        |                                                         |    |\n", widthBorder.c_str());
			printf("%s|   __   |                                                         |    |\n", widthBorder.c_str());
			printf("%s|  |  |  |                                                         |    |\n", widthBorder.c_str());
			printf("%s|  |__|  |                                                         |    |\n", widthBorder.c_str());
			printf("%s|       /     ________     ________     ___    ___     _________   |    |\n", widthBorder.c_str());
			printf("%s|      /     |        |   |        |   |   |  |   |   |    __   |  |    |\n", widthBorder.c_str());
			printf("%s|  |\\  \\     |   __   |   |   __   |   |   |  |   |   |   |__|  |  |____|\n", widthBorder.c_str());
			printf("%s|  | \\  \\    |  |__|  |   |  |__|  |   |   |__|   |   |  _______|   ____\n", widthBorder.c_str());
			printf("%s|  |  \\  \\   |        |   |        |   |          |   |  |______   |    |\n", widthBorder.c_str());
			printf("%s|__|   \\__\\  |________|   |_____   |   |__________|   |_________|  |____|\n", widthBorder.c_str());
			printf("%s                                |  |\n", widthBorder.c_str());
			printf("%s                           _____|  |\n", widthBorder.c_str());
			printf("%s                          |________|\n", widthBorder.c_str());
		}
		else if (screen == 2 || screen == 3 || screen == 4) {
			string space;
			for (int i = 0; i < width - 2; i++)
				space += " ";
			//height is 6 more than it needs to be.
			//for (unsigned i = 0; i < campaigns.size(); i++) {
				//printf("%i. %s which is %i levels long\n\n", i + 1, campaigns[i].c_str(), campaignLengths[i]);
			for (int i = 0; i < height; i++) {
				if (i == 0 || i == height - 1) {
					string top;
					for (int j = 0; j < width; j++) {
						if (j % 2 == 1)
							top += '+';
						else
							top += '-';
					}
					printf("%s%s\n", widthBorder.c_str(), top.c_str());
				}
				else {
					if (i % 2 == 0) {
						string space1 = "", space2 = "";
						int index = (i / 2) - 1;
						for (unsigned i = 0; i < int(campaignSpaceLengths[index] / 2); i++)
							space1 += " ";
						for (unsigned i = 0; i < campaignSpaceLengths[index] - int(campaignSpaceLengths[index] / 2); i++)
							space2 += " ";
						printf("%s|  %s%i. %s, which is %i levels long%s  |\n", widthBorder.c_str(), space1.c_str(), index + 1, campaigns[index].c_str(), campaignLengths[index], space2.c_str());
					}
					else
						printf("%s+%s+\n", widthBorder.c_str(), space.c_str());
				}
				//}
			}
			//Print the new game list
			//maybe with like a nice border or something
		}
		else if (screen == 5) {
			string space;
			for (int i = 0; i < width - 2; i++)
				space += " ";
			//height is 6 more than it needs to be.
			//for (unsigned i = 0; i < campaigns.size(); i++) {
			//printf("%i. %s which is %i levels long\n\n", i + 1, campaigns[i].c_str(), campaignLengths[i]);
			for (int i = 0; i < height; i++) {
				if (i == 0 || i == height - 1) {
					string top;
					for (int j = 0; j < width; j++) {
						if (j % 2 == 1)
							top += '+';
						else
							top += '-';
					}
					printf("%s%s\n", widthBorder.c_str(), top.c_str());
				}
				else {
					if (i % 2 == 0) {
						string space1 = "", space2 = "";
						int index = (i / 2) - 1;
						for (unsigned i = 0; i < int(saveSpaceLengths[index] / 2); i++)
							space1 += " ";
						for (unsigned i = 0; i < saveSpaceLengths[index] - int(saveSpaceLengths[index] / 2); i++)
							space2 += " ";
						printf("%s|  %s%i. %s%s  |\n", widthBorder.c_str(), space1.c_str(), index + 1, saveFiles[index].c_str(), space2.c_str());
						//printf("%s|  %s%i. %s, which is %i levels long%s  |\n", widthBorder.c_str(), space1.c_str(), index + 1, campaigns[index].c_str(), campaignLengths[index], space2.c_str());
					}
					else
						printf("%s+%s+\n", widthBorder.c_str(), space.c_str());
				}
				//}
			}

			//Print the saved game list
			//again, with a nice border?
			//printf("%i. %s\n", i + 1, saveFiles[i].c_str());
		}
		else if (screen == 6) {

			printf("%s$$$$$$$$$$$$$$$$$$$$$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$$$$$$$$$$$$$$$$$$$$$\n", widthBorder.c_str());
			printf("%s$$$'`$$$$$$$$$$$$$'`$$$$$\n", widthBorder.c_str());
			printf("%s$$$$  $$$$$$$$$$$  $$$$$$\n", widthBorder.c_str());
			printf("%s$$$$. `$' \\' \\$`  $$$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$. !\\  i  i .$$$$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$$   `--`--.$$$$$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$$L        `$$$$$^^$$\n", widthBorder.c_str());
			printf("%s$$$$$$$.   .'   \"\"~   $$$\n", widthBorder.c_str());
			printf("%s$$$$$$$$.  ;      .e$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$$$$$   `.$$$$$$$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$$$$    .$$$$$$$$$$$$\n", widthBorder.c_str());
			printf("%s$$$$$$$     $$$$$$$$$$$$$\n", widthBorder.c_str());
			printf("%s-------------------------\n", widthBorder.c_str());
			printf("%s    THANKS FOR PLAYING   \n", widthBorder.c_str());
			printf("%s-------------------------\n", widthBorder.c_str());

		}


		for (int i = 0; i < heightSpace - int(heightSpace / 2); i++)
			printf("\n");
		//Try to also add a delete save file function

		// screen: 1 is ROGUE screen, 2 is new game choosing campaign, 3 is new game choosing name, 4 error,
		//5 is load game choosing saved games, 6 goodbye screen
		widthBorder = "";
		if (screen == 1) {
			for (int i = 0; i < windowWidth/2 - 34; i++)
				widthBorder += " ";
			printf("%sWould you like to start a new game, load an existing game, or exit? ", widthBorder.c_str());
		}
		else if (screen == 2) {
			for (int i = 0; i < windowWidth/2 - 24; i++)
				widthBorder += " ";
			printf("%sChoose your campaign!! Or type \"quit\" to quit... ", widthBorder.c_str());
		}
		else if (screen == 3) {
			for (int i = 0; i < windowWidth/2 - 37; i++)
				widthBorder += " ";
			printf("%sAnd finally... what is your name? Unless, of course, you want to quit... ", widthBorder.c_str());
		}
		else if (screen == 4) {
			for (int i = 0; i < windowWidth/2 - 26; i++)
				widthBorder = +" ";
			printf("%sThat's way too long! 19 letters or shorter please! ", widthBorder.c_str());
		}
		else if (screen == 5) {
			for (int i = 0; i < windowWidth/2 - 33; i++)
				widthBorder += " ";
			printf("%sChoose the saved game you want to load or type \"quit\" to quit... ", widthBorder.c_str());
		}
		if (isPlaying)
			cin >> response;
		if (screen == 1) {
			if (response == "New" || response == "new") {
				//new game;
				//Show a list of campaigns and how long they are;
				screen = 2;
				width = 0;
				height = 0;
				for (unsigned i = 0; i < campaigns.size(); i++) {
					height += 2;
					//printf("%i. %s, which is %i levels long\n\n", i + 1, campaigns[i].c_str(), campaignLengths[i]);
					int len, campLen;
					if (i < 9) len = 1; else if (i < 99) len = 2; else if (i < 999) len = 3;
					if (campaignLengths[i] < 9) campLen = 1; else if (campaignLengths[i] < 99) campLen = 2; else if (campaignLengths[i] < 999) campLen = 3;
					int tempWidth = len + 2 + campaigns[i].length() + 11 + campLen + 12 + 6;
					if (tempWidth > width)
						width = tempWidth;
				}
				//width += 10;
				height += 3;
				for (unsigned i = 0; i < campaigns.size(); i++) {
					int len, campLen;
					if (i < 9) len = 1; else if (i < 99) len = 2; else if (i < 999) len = 3;
					if (campaignLengths[i] < 9) campLen = 1; else if (campaignLengths[i] < 99) campLen = 2; else if (campaignLengths[i] < 999) campLen = 3;
					int tempWidth = len + 2 + campaigns[i].length() + 11 + campLen + 12 + 6;
					campaignSpaceLengths.push_back(width - tempWidth);
				}

				/*
				bool a = true;
				while (a) {
					int response;
					cin >> response;
					if (response > 0 && (unsigned)response <= campaigns.size()) {
						campaign = campaigns[response - 1];
						campaignLength = campaignLengths[response - 1];
						a = false;
					}

				}
				a = true;
				printf("\nAnd finally... what is your name? ");
				while (a) {
					cin >> response;
					if (response.length() > 19)
						cout << "\nThat's way too long! 19 letters or shorter please!";
					else
						a = false;
				}
				playerName = response;
				ifNewGame = true;
				a = false;
				*/
			}
			else if (response == "load" || response == "Load") {
				//load game duh;
				//Show a list of save game files (using saveGameFiles.txt duh)

				screen = 5;

				width = 0;
				height = 0;
				for (unsigned i = 0; i < saveFiles.size(); i++) {
					//printf("%i. %s\n", i + 1, saveFiles[i].c_str());
					int len;
					height += 2;
					if (i < 9) len = 1; else if (i < 99) len = 2; else if (i < 999) len = 3;
					if (len + 2 + saveFiles[i].length() + 6 > width)
						width = len + 2 + saveFiles[i].length() + 6; //plus 6 for the border, two spaces and one character both sides
				}
				height += 3; // plus 3 for the border, one space and one character top, but only character bottom.
				for (unsigned i = 0; i < saveFiles.size(); i++) {
					int len;
					if (i < 9) len = 1; else if (i < 99) len = 2; else if (i < 999) len = 3;
					int tempWidth = len + 2 + saveFiles[i].length() + 6;
					saveSpaceLengths.push_back(width - tempWidth);
				}
				/*
				bool a = true;
				while (a) {
					int response;
					cin >> response;
					if (response > 0 && (unsigned)response <= saveFiles.size()) {
						saveFileName = "SaveFiles/" + saveFiles[response - 1] + ".txt";
						a = false;
					}

				}
				screen = 3;
				ifLoadGame = true;
				a = false;
				*/
			}

			//else if (response == "high score list"); ja feel?
			else if (response == "exit" || response == "Exit") {
				isPlaying = false;
				screen = 6;
				width = 26;
				height = 16;
			}
		}
		else if (screen == 2) {
			stringstream iss(response);
			if (response == "quit" || response == "Quit") {
				screen = 1;
				width = 74;
				height = 15;
			}
			else {
				int num;
				iss >> num;
				if (num > 0 && (unsigned)num <= campaigns.size()) {
					campaign = campaigns[num - 1];
					campaignLength = campaignLengths[num - 1];
					screen = 3;
				}
			}
		}
		else if (screen == 3 || screen == 4) {
			if (response != "quit" && response != "Quit") {
				if (response.length() > 19)
					screen = 4;
				else {
					playerName = response;
					ifNewGame = true;
				}
			}
			else {
				screen = 1;
				width = 74;
				height = 15;
			}
		}
		else if (screen == 5) {
			stringstream iss(response);
			if (response == "quit" || response == "Quit") {
				screen = 1;
				width = 74;
				height = 15;
			}
			else {
				int num;
				iss >> num;
				if (num > 0 && (unsigned)num <= saveFiles.size()) {
					saveFileName = "SaveFiles/" + saveFiles[num - 1] + ".txt";
					ifLoadGame = true;
				}
			}
		}
		else if (screen == 6) {
			isPlaying = false;
			a = false;
		}

		if (ifNewGame || ifLoadGame)
			a = false;
	}
}