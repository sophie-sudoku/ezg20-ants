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
#include "debug.hpp"

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

	GLuint cubemapTexture = SOIL_load_OGL_cubemap(
		"assets/textures/cubemap/px.png", "assets/textures/cubemap/nx.png",
		"assets/textures/cubemap/py.png", "assets/textures/cubemap/ny.png",
		"assets/textures/cubemap/pz.png", "assets/textures/cubemap/nz.png", 
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	GLfloat cubemapVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	/*
	GLuint cubemapVertexbuffer;
	glGenBuffers(1, &cubemapVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubemapVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), cubemapVertices, GL_STATIC_DRAW);
	*/

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("assets/shader/StandardShading.vert", "assets/shader/StandardShading.frag");
	GLuint programID2 = LoadShaders("assets/shader/StandardShading.vert", "assets/shader/StandardShading.frag");
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
	Mesh *ant1 = new Mesh("assets/models/ant_sitting.obj");
	ant1->SetShader(programID, "assets/textures/uvmap.DDS");
	ant1->SetupMesh();
	const std::string output = Json::writeString(builder, positions["ant1"]);
	ant1->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	Mesh* ant2 = new Mesh("assets/models/desert.obj");
	ant2->SetShader(programID2, "assets/textures/desert_diffuse.png");
	ant2->SetupMesh();
	ant2->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	Mesh* ant3 = new Mesh("assets/models/desert.obj");
	ant3->SetShader(programID2, NULL);
	ant3->SetupMesh();
	ant3->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	Mesh* ant4 = new Mesh("assets/models/desert.obj");
	ant4->SetShader(programID2, NULL);
	ant4->SetupMesh();
	ant4->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	Mesh* ant5 = new Mesh("assets/models/desert.obj");
	ant5->SetShader(programID2, NULL);
	ant5->SetupMesh();
	ant5->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	Mesh* ant6 = new Mesh("assets/models/desert.obj");
	ant6->SetShader(programID2, NULL);
	ant6->SetupMesh();
	ant6->SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 0.0f, 0.0f)));

	do {



		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		glUseProgram(cubemapProgram);
		
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		
		glBindVertexArray(cubemapVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		ant1->Draw(
			ProjectionMatrix,
			ViewMatrix
		);
		
		ant2->Draw(
			ProjectionMatrix,
			ViewMatrix
		);

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
	glDeleteProgram(programID);
	glDeleteProgram(programID2);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

