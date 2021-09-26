#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Object
{
public:
	std::string type;
	glm::vec3 position;
};