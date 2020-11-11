// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "json/json.h"
#include <fstream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Include SOIL2
#include <SOIL2.h>

#include "shader.hpp"
#include "controls.hpp"
#include "vboindexer.hpp"
#include "mesh.hpp"
#include "utils.hpp"
#include "cubemap.hpp"

#include <sstream>

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Ants", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Setup scene settings
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint antProgram = LoadShaders("assets/shader/StandardShading.vert", "assets/shader/StandardShading.frag");
	GLuint desertProgram = LoadShaders("assets/shader/StandardShading.vert", "assets/shader/StandardShading.frag");
	GLuint cubemapProgram = LoadShaders("assets/shader/CubemapShader.vert", "assets/shader/CubemapShader.frag");

	// Get model positions from positions.json
	std::ifstream positions_file("assets/models/positions.json");
	std::ostringstream tmp;
	tmp << positions_file.rdbuf();
	std::string spositions_file_str = tmp.str();

	Json::Value positions;
	std::istringstream sin(spositions_file_str);
	sin >> positions;
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "";

	// Load scene
	Mesh* desert = new Mesh("assets/models/desert.obj");
	desert->SetShader(desertProgram, "assets/textures/desert_diffuse.png");
	desert->SetupMesh();
	desert->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	
	Mesh *ant = new Mesh("assets/models/ant_sitting.obj");
	ant->SetShader(antProgram, "assets/textures/uvmap.DDS");
	ant->SetupMesh();
	
	
	const std::string output = Json::writeString(builder, positions["test"]);
	makeMat4(output);
	
	std::vector<glm::mat4> antPositions;
	antPositions.push_back( glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)) );
	antPositions.push_back( glm::translate(glm::mat4(1.0), glm::vec3(4.0f, 0.0f, 0.0f)) );
	antPositions.push_back( glm::translate(glm::mat4(1.0), glm::vec3(6.0f, 0.0f, 0.0f)) );

	//Cubemap* sky = new Cubemap("cubemap", 1.0f, cubemapProgram);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		
		/*
		sky->Draw(
			ProjectionMatrix,
			ViewMatrix
		);
		*/
		
		//Draw Desert
		desert->Draw(
			ProjectionMatrix,
			ViewMatrix
		);

		//Draw Ants
		for (auto &position : antPositions) {
			ant->SetTransform(position);
			ant->Draw(
				ProjectionMatrix,
				ViewMatrix
			);
		}


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteProgram(antProgram);
	glDeleteProgram(desertProgram);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Clean up Meshes
	delete desert;
	delete ant;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

