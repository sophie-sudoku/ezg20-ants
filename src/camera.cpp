// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "camera.hpp"


void Camera::setupCamera(bool userControl) {

	// Initial position : on +Z
	this->position = glm::vec3(14.75, 9, 25);
	// Initial horizontal angle : toward -Z
	this->horizontalAngle = -2.17f;
	// Initial vertical angle : none
	this->verticalAngle = 6.06f;
	// Initial Field of View
	this->initialFoV = 45.0f;


	if (userControl) {
		//For Debugging only
		this->orbit = false;
		this->orbitRadius = 10.0f;
		this->F_currently_pressed = false;
		this->initialMousePosition = true;
		this->speed = 3.0f; // 3 units / second
		this->mouseSpeed = 0.005f;
	}

	else {

		std::vector<vec3> positions;
		std::vector<float> intervals;

		//Set up positions and interpolation rate between them

		//First position is only used for interpolation
		positions.push_back(this->position + vec3(3, 3, 3));


		//First real position
		positions.push_back(this->position);
		intervals.push_back(0.005);
		positions.push_back(vec3(0, 2, 0));
		intervals.push_back(0.005);
		positions.push_back(vec3(5, 3, 0));
		intervals.push_back(0.005);
		positions.push_back(vec3(5, 4, 5));
		intervals.push_back(0.005);
		positions.push_back(this->position);
		intervals.push_back(0.005);
		positions.push_back(vec3(0, 2, 0));
		intervals.push_back(0.005);
		positions.push_back(vec3(5, 3, 0));
		intervals.push_back(0.005);
		positions.push_back(vec3(5, 4, 5));
		intervals.push_back(0.005);
		positions.push_back(this->position);
		intervals.push_back(0.005);

		//Last position is only used for interpolation
		positions.push_back(this->position + vec3(3, 3, 3));


		addInterpPositions(GetParabolaInterpSpline(positions, intervals));
		
		
		currentInterpPos = 0;

		//TODO: Set up angles and linearly interpolate between them

	}
}

void Camera::updateCamera(float dt) {

	// Interpolate to next position, TODO: based on dt 
	this->position = interpPositions[currentInterpPos];
	currentInterpPos++;
	if (currentInterpPos >= interpPositions.size()) { currentInterpPos = 0; }

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


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		this->position,      // Camera is here
		this->position + direction,		// and looks here : at the same position, plus "direction"
		up					// Head is up (set to 0,-1,0 to look upside-down)
	);
}


void Camera::computeMatricesFromInputs(float dt) {

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
			position += direction * dt * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * dt * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * dt * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * dt * speed;
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
		finalPosition,      // Camera is here
		finalDirection,		// and looks here : at the same position, plus "direction"
		up					// Head is up (set to 0,-1,0 to look upside-down)
	);




}


mat4 Camera::getViewMatrix() {
	return ViewMatrix;
}
mat4 Camera::getProjectionMatrix() {
	return ProjectionMatrix;
}

void Camera::addInterpPositions(std::vector<vec3> interpolations) {
	this->interpPositions.insert(this->interpPositions.end(), interpolations.begin(), interpolations.end());
}



// Blended Parabola Interpolation
// Follwoing code from Assignment 2 of Computeranimation in SS2020 by Jonas Prohaska

std::vector<vec3> Camera::GetParabolaInterpSpline(std::vector<vec3> positions, std::vector<float> intervals)
{

	std::vector<vec3> interpList;

	if (positions.size() < 4) {
		return interpList;
	}
	for (int i = 1; i < positions.size() - 2; i++) {

		glm::mat4 parabolaXpoints = ConstructParabolaB(
			positions[i - 1], 
			positions[i], 
			positions[i + 1], 
			positions[i + 2]);

		//for interval, interpolate according to theory
		for (float intervalpos = 0; intervalpos <= 1; intervalpos += intervals[i-1]) {
			vec4 intervalscalar = vec4(pow(intervalpos, 3), pow(intervalpos, 2), intervalpos, 1);
			interpList.push_back(vec3(multiply1x4by4x4(intervalscalar, parabolaXpoints)));
		}
	}

	//add last position to end off list
	interpList.push_back(positions[positions.size() - 2]);
	return interpList;
}

mat4 Camera::ConstructParabolaB(vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
	//Blended Parabola Formula
	glm::mat4 parabolamatrix = glm::mat4(
		vec4(-1.f, 3.f, -3.f, 1.f),
		vec4(2.f, -5.f, 4.f, -1.f),
		vec4(-1.f, 0.f, 1.f, 0.f),
		vec4(0.f, 2.f, 0.f, 0.f));
	parabolamatrix *= 0.5f;
	glm::mat4 pointmatrix = glm::mat4(
		p1.x, p1.y, p1.z, 0.f,
		p2.x, p2.y, p2.z, 0.f,
		p3.x, p3.y, p3.z, 0.f,
		p4.x, p4.y, p4.z, 0.f);
	return multiply4x4by4x4(parabolamatrix, pointmatrix);
}

vec4 Camera::multiply1x4by4x4(vec4 a, glm::mat4 b) {
	//probably a glm function that does this, but didnt find it at the time
	//since the interpolation is only done on startup it doesn't affect realtime performance
	return vec4(
		a[0] * b[0][0] + a[1] * b[1][0] + a[2] * b[2][0] + a[3] * b[3][0],
		a[0] * b[0][1] + a[1] * b[1][1] + a[2] * b[2][1] + a[3] * b[3][1],
		a[0] * b[0][2] + a[1] * b[1][2] + a[2] * b[2][2] + a[3] * b[3][2],
		a[0] * b[0][3] + a[1] * b[1][3] + a[2] * b[2][3] + a[3] * b[3][3]

	);
}

mat4 Camera::multiply4x4by4x4(glm::mat4 a, glm::mat4 b) {
	//probably a glm function that does this, but didnt find it at the time, 
	//since the interpolation is only done on startup it doesn't affect realtime performance
	return mat4(
		a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0],
		a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1],
		a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2],
		a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3],

		a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0],
		a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1],
		a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2],
		a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3],

		a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0],
		a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1],
		a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2],
		a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3],

		a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0],
		a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1],
		a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2],
		a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3]

	);
}

//copied code end