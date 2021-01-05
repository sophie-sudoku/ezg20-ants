// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <irrKlang.h>

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
#include "particles.hpp"
#include "utils.hpp"
#include "cubemap.hpp"
#include "shadowmap.hpp"

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

	// Create and compile our GLSL program from the shaders
	GLuint standardProgram = LoadShaders("assets/shader/StandardShading.vert", "assets/shader/StandardShading.frag");
	GLuint cubemapProgram = LoadShaders("assets/shader/CubemapShader.vert", "assets/shader/CubemapShader.frag");
	GLuint particleProgram = LoadShaders("assets/shader/ParticleShader.vert", "assets/shader/ParticleShader.frag");

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

	glm::vec3 defaultColor = glm::vec3(0.0, 0.0, 0.0);

	// Load scene
	Mesh* desert = new Mesh("assets/models/desert.obj");
	desert->SetShader(standardProgram, "assets/textures/desert_diffuse.png", defaultColor);
	desert->SetupMesh();
	
	Mesh *ant = new Mesh("assets/models/ant_sitting.obj");
	ant->SetShader(standardProgram, "", glm::vec3(0.2, 0.0, 0.0));
	ant->SetupMesh();

	std::vector<glm::mat4> antPositions;
	antPositions.push_back(makeMat4(Json::writeString(builder, positions["ant_1"])));
	antPositions.push_back(makeMat4(Json::writeString(builder, positions["ant_2"])));
	antPositions.push_back(makeMat4(Json::writeString(builder, positions["ant_3"])));
	antPositions.push_back(makeMat4(Json::writeString(builder, positions["ant_4"])));
	antPositions.push_back(makeMat4(Json::writeString(builder, positions["ant_5"])));
	antPositions.push_back(makeMat4(Json::writeString(builder, positions["ant_6"])));

	Mesh* cactus = new Mesh("assets/models/cactus.obj");
	cactus->SetShader(standardProgram, "assets/textures/cactus_albedo.png", defaultColor);
	cactus->SetupMesh();

	std::vector<glm::mat4> cactusPositions;
	cactusPositions.push_back(makeMat4(Json::writeString(builder, positions["cactus_1"])));
	cactusPositions.push_back(makeMat4(Json::writeString(builder, positions["cactus_2"])));
	cactusPositions.push_back(makeMat4(Json::writeString(builder, positions["cactus_3"])));

	Mesh* log = new Mesh("assets/models/log.obj");
	log->SetShader(standardProgram, "", glm::vec3(0.543, 0.27, 0.074));
	log->SetupMesh();

	Mesh* stones = new Mesh("assets/models/stones.obj");
	stones->SetShader(standardProgram, "", glm::vec3(0.2, 0.2, 0.2));
	stones->SetupMesh();

	Mesh* carpet = new Mesh("assets/models/carpet.obj");
	carpet->SetShader(standardProgram, "assets/textures/carpet_diffuse.png", defaultColor);
	carpet->SetupMesh();

	std::vector<glm::mat4> grasPositions;
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_1"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_2"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_3"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_4"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_5"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_6"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_7"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_8"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_9"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_10"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_11"])));
	grasPositions.push_back(makeMat4(Json::writeString(builder, positions["gras_12"])));
	Mesh* gras = new Mesh("assets/models/gras.obj");
	gras->SetShader(standardProgram, "assets/textures/gras.png", defaultColor);
	gras->SetupMesh();

	std::vector<glm::mat4> stonePositions;
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_1"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_2"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_3"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_4"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_5"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_6"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_7"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_8"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_9"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_10"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_11"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_12"])));
	stonePositions.push_back(makeMat4(Json::writeString(builder, positions["stones_13"])));
	Mesh* stone = new Mesh("assets/models/stone.obj");
	stone->SetShader(standardProgram, "", glm::vec3(0.2, 0.2, 0.2));
	stone->SetupMesh();

	
	unsigned int numberOfParticles = 500;
	unsigned int newParticlesPerFrame = 5;
	ParticleSystem* ps = new ParticleSystem(numberOfParticles, newParticlesPerFrame, particleProgram);

	Cubemap* sky = new Cubemap("cubemap", 1.0f, cubemapProgram);
	Shadowmap* shadowmap = new Shadowmap();

	std::vector<Mesh*> shadowMeshes = {desert, ant, stone, carpet};

	for (Mesh* mesh : shadowMeshes) {
		mesh->SetDepthTexture(shadowmap->depthTexture);
	}

	// Lightsource: Fire
	glm::vec3 lightPos = glm::vec3(-2.8, 1.2, 4.5);
	uint frame = 1.0;
	bool reverse = false;

	// Create sound
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	SoundEngine->play2D("assets/audio/guitar-track.mp3", true);

	do {
		// Animate light
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_real_distribution<float> distr(-0.1, 0.1);

		if (reverse) {
			frame = frame -1;
		} else {
			frame = frame +1;
		}
		if (frame <= 0 || frame >= 80) {
			reverse = !reverse;
		}
		// Remove +distr(eng) if the flickering is disturbing
		lightPos.y = (0.8 + (frame / 80.0))+distr(eng);

		// Draw shadows to shadow map
		shadowmap->DrawSetup();
		
		for (auto& position : stonePositions) {
			stone->SetTransform(position);
			shadowmap->Draw(stone, lightPos);
		}
		for (auto& position : antPositions) {
			ant->SetTransform(position);
			shadowmap->Draw(ant, lightPos);
		}

		shadowmap->Draw(desert, lightPos);
		shadowmap->Draw(carpet, lightPos);
		shadowmap->DrawTeardown();


		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		
		//Draw cubemap
		sky->Draw(
			ProjectionMatrix,
			ViewMatrix
		);
		//Draw Desert
		desert->Draw(
			ProjectionMatrix,
			ViewMatrix,
			lightPos,
			shadowmap->depthBiasMVP
		);

		//Draw carpet
		carpet->Draw(
			ProjectionMatrix,
			ViewMatrix,
			lightPos,
			shadowmap->depthBiasMVP
		);

		//Draw grass
		for (auto& position : grasPositions) {
			gras->SetTransform(position);
			gras->Draw(
				ProjectionMatrix,
				ViewMatrix,
				lightPos,
				shadowmap->depthBiasMVP
			);
		}

		//Draw single stones
		for (auto& position : stonePositions) {
			stone->SetTransform(position);
			stone->Draw(
				ProjectionMatrix,
				ViewMatrix,
				lightPos,
				shadowmap->depthBiasMVP
			);
		}

		//Draw Ants
		for (auto &position : antPositions) {
			ant->SetTransform(position);
			ant->Draw(
				ProjectionMatrix,
				ViewMatrix,
				lightPos,
				shadowmap->depthBiasMVP
			);
		}

		//Draw Cacti
		for (auto& position : cactusPositions) {
			cactus->SetTransform(position);
			cactus->Draw(
				ProjectionMatrix,
				ViewMatrix,
				lightPos,
				shadowmap->depthBiasMVP
			);
		}

		//Draw Stones
		stones->Draw(
			ProjectionMatrix,
			ViewMatrix,
			lightPos,
			shadowmap->depthBiasMVP
		);

		//Draw Log
		log->Draw(
			ProjectionMatrix,
			ViewMatrix,
			lightPos,
			shadowmap->depthBiasMVP
		);


		//Draw Particles
		ps->UpdateParticles();
		ps->DrawParticles();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// TODO: Cleanup shaders
	glDeleteProgram(standardProgram);

	// TODO: Clean up Meshes
	delete desert;
	delete ant;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
