#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "Level.h"
#include "Wall.h"
#include "Object.h"
#include "string"
#include <vector>
#include <sstream>


//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void Character_callback(GLFWwindow* window, unsigned int codepoint);
void SplitWords(string str);
void DoCommands(vector<string> _commands, GLFWwindow* window);
void LoadCommand(vector<string> _commands);
void SpawnCommand(vector<string> _commands);
void HelpCommand();
void FPSCommand();
void TriangleCommand();
void CountPolys();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(3.0f, 2.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const int textureType = 0;

//Console
bool consoleActivated = false;

vector<Model> models;
vector<unsigned int> textures;
string commandInput = "";
vector<string> commands;

//Make level and receive object array
Level level;
double lastTime = glfwGetTime();
int nbFrames = 0;
bool displayFPS = false;
int triangles = 0;

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

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//Shaders
	Shader ourShader("Shader.vert", "Shader.frag");
	Shader lightCubeShader("LightCube.vert", "LightCube.frag");

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
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f,

		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,

		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

		 //Bottom
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

		//Top
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
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		 0.5f,  1.2f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		-0.5f,  1.2f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f,

		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  1.2f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  1.2f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  1.2f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,

		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  1.2f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.2f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.2f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

		 //Bottom
		-0.5f,  1.2f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  1.2f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  1.2f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  1.2f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  1.2f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  1.2f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

		//Top
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

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ourShader.use();
	ourShader.setInt("depthMap", 1);

	
	for (size_t i = 0; i < level.textures->size(); i++)
	{
		std::string path = "Textures/" + level.textures[i];
		textures.push_back(loadTexture(path.c_str()));
	}


	for (size_t i = 0; i < level.models.size(); i++)
	{
		Model ourModel(level.models[i].path);
		models.push_back(ourModel);
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		triangles = 0;
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

		ourShader.use();

		for (size_t i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
		
		//View/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		//Adding camera position to shader
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("material.shininess", 32.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Adding directional light to shader
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		//Adding spotLight to shader
		ourShader.setVec3("spotLight.position", camera.Position);
		ourShader.setVec3("spotLight.direction", camera.Front);
		ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("spotLight.constant", 1.0f);
		ourShader.setFloat("spotLight.linear", 0.09);
		ourShader.setFloat("spotLight.quadratic", 0.032);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//Adding all point lights to the shader
		for (size_t i = 0; i < level.lights.size(); i++)
		{
			// point light
			ourShader.setVec3("pointLights[" + to_string(i) + "].position", level.lights.at(i).position);
			ourShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
			ourShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.09);
			ourShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032);
		}

		glm::mat4 model = glm::mat4(1.0f);

		glBindVertexArray(cubeVAO);

		//Running though level's walls and drawing
		for (unsigned int i = 0; i < level.walls.size(); i++)
		{
			ourShader.setInt("material.diffuse", level.walls.at(i).texture);
			ourShader.setInt("material.specular", level.walls.at(i).texture);
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, level.walls.at(i).position);
			ourShader.setMat4("model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	
		//Running though level's doors and drawing
		for (unsigned int i = 0; i < level.doors.size(); i++)
		{
			ourShader.setInt("material.diffuse", level.doors.at(i).texture);
			ourShader.setInt("material.specular", level.doors.at(i).texture);
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, level.doors.at(i).position);
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 36, 36);
		}

		for (unsigned int i = 0; i < level.floors.size(); i++)
		{  
			ourShader.setInt("material.diffuse", 6);
			ourShader.setInt("material.specular", 6);
			model = glm::mat4(1.0f);
			model = glm::translate(model, level.floors.at(i).position);
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 30, 6);
		}

		glDisable(GL_CULL_FACE);
		
		ourShader.setInt("material.diffuse", 0);
		for (size_t i = 0; i < level.models.size(); i++)
		{
			// render the loaded model
			model = glm::mat4(1.0f);
			model = glm::translate(model, level.models[i].position); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			models[i].Draw(ourShader);
		}

		for (unsigned int i = 0; i < level.lights.size(); i++)
		{
			// also draw the lamp object
			lightCubeShader.use();
			lightCubeShader.setMat4("projection", projection);
			lightCubeShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			model = glm::translate(model, level.lights.at(i).position);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lightCubeShader.setMat4("model", model);

			glBindVertexArray(lightCubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Calculate FPS
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			if (displayFPS) {
				cout << "Frames per second: ";
				cout << nbFrames << endl;
				cout << endl;
				displayFPS = false;
			}

			nbFrames = 0;
			lastTime += 1.0;
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

	

	if (!consoleActivated) {
		//Movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
			consoleActivated = true;
			commands.clear();
			cout << "-----------------------------------" << endl;
			cout << "Console Activiated" << endl;
			glfwSetCharCallback(window, Character_callback);
		}
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			consoleActivated = false;
			SplitWords(commandInput);
			cout << "\n";
			DoCommands(commands, window);
			glfwSetCharCallback(window, NULL);
			commands.clear();
			commandInput = "";
		}
	}
}

void Character_callback(GLFWwindow* window, unsigned int codepoint) {
	cout << (char)codepoint;
	commandInput += (char)codepoint;
}

void DoCommands(vector<string> _commands, GLFWwindow* window) {
	if (_commands[0] == "FPS")
		FPSCommand();
	else if (_commands[0] == "LOAD")
		LoadCommand(_commands);
	else if (_commands[0] == "SPAWN")
		SpawnCommand(_commands);
	else if (_commands[0] == "TRIANGLES")
		TriangleCommand();
	else if (_commands[0] == "QUIT")
		glfwSetWindowShouldClose(window, true);
	else if (_commands[0] == "HELP")
		HelpCommand();
	else
	{
		cout << "Unknown command" << endl;
		HelpCommand();
	}
}

void FPSCommand() {
	displayFPS = true;
}

void LoadCommand(vector<string> _commands) {
	if (_commands.size() >= 2) {
		Level newLevel(_commands[1]);

		if (newLevel.LoadLevel()) {
			cout << "Loading level..." << endl;
			cout << "Loading doors..." << endl;
			level.doors = newLevel.doors;
			cout << "Loading walls..." << endl;
			level.walls = newLevel.walls;
			cout << "Loading floors..." << endl;
			level.floors = newLevel.floors;
			cout << "Loading models..." << endl;
			level.models = newLevel.models;
			cout << "Loading lights..." << endl;
			level.lights = newLevel.lights;
			cout << "Level loaded.\n" << endl;
		}
		else {
			cout << "Could not find level file with that name\n" << endl;
		}
	}
	else {
		cout << "Incorrect use of command." << endl;
		HelpCommand();
	}
}

void TriangleCommand() {
	CountPolys();

	cout << "Triangles: ";
	cout << triangles;
}

void SpawnCommand(vector<string> _commands) {

	if (_commands.size() == 4) {
		Model ourModel("Models/" + _commands[1]);

		float x = 0;
		stringstream xPos(_commands[2]);
		xPos >> x;

		ModelClass newModel(x, x, "Models/" + _commands[1]);

		level.models.push_back(newModel);
		models.push_back(ourModel);
	}
	else {
		cout << "Incorrect command." << endl;
		cout << "\nHelp:" << endl;
		HelpCommand();
	}

}

void HelpCommand() {
	cout << "Available commands:\nFPS (Displays the current FPS)\nLoad [Level name] (Loads the level)\nSpawn [Model name] [PosX PosZ] (Spawn the model in the world)" << endl;
}

void SplitWords(string str)
{
	string word = "";
	for (auto x : str)
	{
		if (x == ' ' || x == ',')
		{
			commands.push_back(word);
			word = "";
		}
		else {
			word = word + (char)toupper(x);
		}
	}
	commands.push_back(word);
}

void CountPolys() {
	int indices = 0;

	for (auto model : models)
	{
		for (auto mesh : model.meshes)
		{
			indices += mesh.indices.size();
		}
	}

	triangles += indices / 3;
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

// utility function for loading a 2D texture from file
	// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}