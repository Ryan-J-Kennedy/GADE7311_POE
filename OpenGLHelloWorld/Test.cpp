#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Level.h"
#include "Wall.h"
#include "Object.h"
#include "string"

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int main();
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const int textureType = 9;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GADE_POE", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	//Make level and receive object array
	Level firstLevel;

	Shader ourShader("Shader.vert", "Shader.frag");
	Shader lightCubeShader("LightCube.vert", "LightCube.frag");
	

	//float* vertices = firstLevel.objects[0].GetVertices();
	//cout << walls[1].position.x;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Cube	
		//Sides
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

		 //Top
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,

		//Bottom
		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,

		//Door
		//Sides
		-0.5f,  1.2f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
		 0.5f,  1.2f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
		-0.5f,  1.2f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

		-0.5f,  1.2f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
		 0.5f,  1.2f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
		-0.5f,  1.2f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  1.2f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.2f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.2f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  1.2f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.2f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.2f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		
		 //Top
		-0.5f,  1.2f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.2f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  1.2f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.2f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.2f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
		-0.5f,  1.2f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,

		//Bottom
		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
	};

	//Cube VAO and VBO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	std::string pos = "Textures/" + firstLevel.textures[textureType];
	const char* c = pos.c_str();
	unsigned char* data = stbi_load(c , &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use(); // don't forget to activate the shader before setting uniforms!  
	glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture"), 0); // set it manually

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		ourShader.use();
		/*ourShader.setVec3("objectColour", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("lightColour", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("lightPos", firstLevel.lights[2].position);
		ourShader.setVec3("viewPos", camera.Position);*/

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the cube
		glBindVertexArray(cubeVAO);
		glm::mat4 model = glm::mat4(1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (unsigned int i = 0; i < firstLevel.walls.size(); i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, firstLevel.walls.at(i).position);
			ourShader.setMat4("model", model);
			ourShader.setVec3("objectColour", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("lightColour", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("lightPos", firstLevel.lights.at(0).position);
			ourShader.setVec3("viewPos", camera.Position);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (unsigned int i = 0; i < firstLevel.floors.size(); i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, firstLevel.floors.at(i).position);
			ourShader.setMat4("model", model);
			ourShader.setVec3("objectColour", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("lightColour", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("lightPos", firstLevel.lights.at(0).position);
			ourShader.setVec3("viewPos", camera.Position);
			glDrawArrays(GL_TRIANGLES, 30, 6);
		}

		for (unsigned int i = 0; i < firstLevel.doors.size(); i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, firstLevel.doors.at(i).position);
			ourShader.setMat4("model", model);
			ourShader.setVec3("objectColour", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("lightColour", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("lightPos", firstLevel.lights.at(0).position);
			ourShader.setVec3("viewPos", camera.Position);
			glDrawArrays(GL_TRIANGLES, 36, 36);
		}

		for (unsigned int i = 0; i < firstLevel.lights.size(); i++)
		{
			// also draw the lamp object
			lightCubeShader.use();
			lightCubeShader.setMat4("projection", projection);
			lightCubeShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			model = glm::translate(model, firstLevel.lights.at(i).position);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lightCubeShader.setMat4("model", model);

			glBindVertexArray(lightCubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}