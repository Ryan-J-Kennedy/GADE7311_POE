#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <OpenGLHelloWorld/stb_image.h>
#include <iostream>
#include "Object.h"

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


class Door : public Object
{
public:

	int texture;

	Door() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Door(float posX, float posZ, int _texture) {
		position = glm::vec3(posX, 0.0f, posZ);
		texture = _texture;
	}

	float* GetVertices() {
		static float vertices[] = {
			//Positions				//Normal				//Texture
			//Sides
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
			 0.5f,  1.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
			 0.5f,  1.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
			-0.5f,  1.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 0.0f,
			 0.5f,  1.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
			 0.5f,  1.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
			-0.5f,  1.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,

			-0.5f,  1.5f,  0.5f,    -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  1.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f,  1.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

			 0.5f,  1.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
			 0.5f,  1.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			 0.5f,  1.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,

			 //Top
			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,

			//Bottom
			-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f
		};
		return vertices;
	}
};



