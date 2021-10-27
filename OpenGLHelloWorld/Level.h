#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Object.h"
#include "Wall.h"
#include "Floor.h"
#include "Light.h"
#include "Door.h"
#include "ModelClass.h"
using namespace std;

class Level
{
	ifstream inFile;

public:
	static const unsigned int width = 29;
	static const unsigned int height = 15;
	static const int levelSize = width * height;
	char vecArr[levelSize] = { NULL };
	vector<Wall> walls;
	vector<Floor> floors;
	vector<Light> lights;
	vector<Door> doors;
	vector<ModelClass> models;
	string levelName = "Level1";
	bool canLoad = true;

	std::string textures[10] = { "blue.jpg",  "blueSquaress.jpg", "Bricks.jpg", "camo.jpg", "colours.jpg", "ivy.jpg", "laminate.jpg", "marbleColour.jpg", "metal.jpg", "wood.jpg" };

	Level() {
		ReadFromFile();
	}

	Level(string _levelName) {
		levelName = _levelName;
	}
	
	bool LoadLevel() {
		ReadFromFile();
		return canLoad;
	}

	void ReadFromFile() {

		inFile.open("Levels/" + levelName + ".txt");

		if (!inFile) {
			canLoad = false;
			//exit(1);   // call system to stop
		}
		else {
			canLoad = true;
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
	}

	void CreateObjects() {
		int posX = 0;
		int posY = 0;

		for (size_t i = 0; i < levelSize; i++)
		{
			
			if (vecArr[i] == 'B' || vecArr[i] == 'I') {
				int texture;
				if (vecArr[i] == 'B')
					texture = 2;
				else
					texture = 5;

				Wall w(posX, posY, texture);
				walls.push_back(w);
			}
			else if (vecArr[i] == 'L') {
				Light l(posX, posY);
				lights.push_back(l);
			}
			else if (vecArr[i] == 'b' || vecArr[i] == 'i') {
				int texture;
				if (vecArr[i] == 'b')
					texture = 2;
				else
					texture = 5;

				Door l(posX, posY, texture);
				doors.push_back(l);
			}
			else if (vecArr[i] == 'G') {
				ModelClass m(posX, posY, "Models/Backpack/backpack.obj");
				models.push_back(m);
			}

			if (vecArr[i] != 'B' && vecArr[i] != 'I' && vecArr[i] != ',') {
				Floor f(posX, posY);
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
	


