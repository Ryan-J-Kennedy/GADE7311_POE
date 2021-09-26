#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Object.h"
#include "Wall.h"
#include "Floor.h"
#include "Light.h"
#include "Door.h"
using namespace std;

class Level
{
	ifstream inFile;

public:
	static const unsigned int width = 15;
	static const unsigned int height = 15;
	static const int levelSize = width * height;
	char vecArr[levelSize] = { NULL };
	vector<Wall> walls;
	vector<Floor> floors;
	vector<Light> lights;
	vector<Door> doors;

	std::string textures[10] = { "blue.jpg",  "blueSquaress.jpg", "Bricks.jpg", "camo.jpg", "colours.jpg", "ivy.jpg", "laminate.jpg", "marbleColour.jpg", "metal.jpg", "wood.jpg" };

	Level() {
		ReadFromFile();
	}

	void ReadFromFile() {
		inFile.open("Level.txt");
		if (!inFile) {
			cerr << "Unable to open file datafile.txt";
			exit(1);   // call system to stop
		}

		string levelString;
		int num = 0;
		while (inFile >> levelString) {
			vector<char> tempChar(levelString.begin(), levelString.end());

			for (size_t i = 0; i < width; i++)
			{
				vecArr[num] = tempChar[i];
				num++;
			}
		}
		CreateObjects();
	}

	void CreateObjects() {
		int posX = 0;
		int posY = 0;

		for (size_t i = 0; i < levelSize; i++)
		{
			if (vecArr[i] == 'W') {
				Wall w(posX, posY, "Wall");
				walls.push_back(w);
			}
			else if (vecArr[i] == 'L') {
				Light l(posX, posY, "Light");
				lights.push_back(l);
			}
			else if (vecArr[i] == 'D') {
				Door l(posX, posY, "Door");
				doors.push_back(l);
			}

			if (vecArr[i] != 'W') {
				Floor f(posX, posY, "Floor");
				floors.push_back(f);
			}
			

			posX++;
			if (posX % width == 0) {
				posX = 0;
				posY++;
			}
		}
	}
};

