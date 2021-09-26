#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Object.h"
#include <string>

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Floor : public Object
{
public:
	Floor() {
		position = glm::vec3(0.0f, -2.0f, 0.0f);
	}

	Floor(float posX, float posZ, std::string _type) {
		position = glm::vec3(posX, -2.0f, posZ);
		type = _type;
	}

	//float* GetVertices() {
	//	static float vertices[] = {
	//		//Positions				//Normal				//Texture
	//		//Sides
	//		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
	//		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
	//		 0.5f,  1.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	//		 0.5f,  1.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	//		-0.5f,  1.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
	//		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

	//		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
	//		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 0.0f,
	//		 0.5f,  1.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
	//		 0.5f,  1.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
	//		-0.5f,  1.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f,
	//		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,

	//		-0.5f,  1.5f,  0.5f,    -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	//		-0.5f,  1.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	//		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	//		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	//		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	//		-0.5f,  1.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

	//		 0.5f,  1.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	//		 0.5f,  1.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	//		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	//		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	//		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	//		 0.5f,  1.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,

	//		 //Top
	//		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	//		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	//		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	//		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	//		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	//		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,

	//		//Bottom
	//		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	//		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	//		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	//		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	//		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	//		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f
	//	};
	//	return vertices;
	//}
};

