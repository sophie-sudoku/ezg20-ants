// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


vec3 finalPosition = vec3(0,0,0);
vec3 finalDirection = vec3(0,0,0);

// Initial position : on +Z
glm::vec3 position = glm::vec3(14.75, 9, 25);
// Initial horizontal angle : toward -Z
float horizontalAngle = -2.17f;
// Initial vertical angle : none
float verticalAngle = 6.06f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;


bool orbit = false; //TODO: Set to false for first prototype
float orbitRadius = 10.0f;
bool F_currently_pressed = false;


bool initialMousePosition = true;	//To ignore initial mouse position affecting the camera (although moving 
									//the mouse during the white screen loading still affects it; could be
									//fixed by only starting this class after loading is done, but a manual
									//camera will not be in the final release anyway, so was decided against)


void computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Compute new orientation based on mouse
	if (!initialMousePosition) { //see comment in global variable
		horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		verticalAngle += mouseSpeed * float(768 / 2 - ypos);
	}
	else { initialMousePosition = false; }
	
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);



	//Get F key to switch between user-controlled and orbit view
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !F_currently_pressed) {
		F_currently_pressed = true;
		glfwSetTime(0);
		orbit = !orbit;
	}
	else {
		F_currently_pressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		printf("break");
		printf("Horizontal Angle: %f ,", horizontalAngle);
		printf("Vertical Angle: %f ,", verticalAngle);
		printf("X: %f ,", position.x);
		printf("Y: %f ,", position.y);
		printf("Z: %f ,", position.z);
		printf("xpos: %f ,", xpos);
		printf("ypos: %f ,", ypos);
		printf("go on");
	}


	if (orbit) { //orbit camera

		//Go up
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position.y += 0.01;
		}
		//Go down
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position.y -= 0.01;
		}
		//Zoom closer
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			orbitRadius -= orbitRadius / 10000;
		}
		//Zoom farer
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			orbitRadius += orbitRadius / 10000;
		}


		//update Position/Direction depending on orbit
		finalPosition = vec3( sin(glfwGetTime()) * orbitRadius,	position.y, cos(glfwGetTime()) * orbitRadius );
		finalDirection = vec3(0, 0, 0);
	}

	else { //user controlled

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}


		//update Position/Direction depending on user controlled camera
		finalPosition = position;
		finalDirection = position + direction;
	}


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		finalPosition,           // Camera is here
		finalDirection, // and looks here : at the same position, plus "direction"
		up                // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}