// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "camera.hpp"


void Camera::setupCamera(float FoV, bool userControl) {

	// Initial position : on +Z
	this->position = glm::vec3(14.75, 9, 25);
	// Initial horizontal angle : toward -Z
	this->horizontalAngle = -2.17f;
	// Initial vertical angle : none
	this->verticalAngle = 6.06f;
	// Initial Field of View
	this->FoV = FoV;


	if (userControl) {
		//For Debugging only
		this->orbit = false;
		this->orbitRadius = 10.0f;
		this->F_currently_pressed = false;
		this->Cpressed = false;
		this->initialMousePosition = true;
		this->speed = 3.0f; // 3 units / second
		this->mouseSpeed = 0.005f;
	}

	else {

		std::vector<vec3> positions;
		std::vector<vec3> angles; //x: horizontal, y: vertical, z: fov
		std::vector<float> intervals;

		//addCut sends the position to be interpolated, so that "cuts" can be made between them
		auto addCut = [&]() {
				addInterpPositions( GetParabolaInterpolation(positions, intervals) );
				addInterpAngles( GetParabolaInterpolation(angles, intervals) );

				positions.clear();
				angles.clear();
				intervals.clear();
		};

		//Set up positions, angles and interpolation rate between them

		//First and last position is only used for blended parabola interpolation
		positions.push_back(this->position);
		angles.push_back(vec3(this->horizontalAngle, this->verticalAngle, this->FoV));

		//First real position
		positions.push_back(vec3(10.87, -4.12, 21.4));
		angles.push_back(vec3(-1.87, 7.05, this->FoV));
		
		//Interval to interpolate between positions (less = slower transistion between positions)
		intervals.push_back(0.005);

		positions.push_back(vec3(6.32, 3.69, 26.99));
		angles.push_back(vec3(-2.47, 6.71, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-0.24, 9.74, 26.52));
		angles.push_back(vec3(-3.32, 6.15, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-11.35, 4.53, 10.68));
		angles.push_back(vec3(-4.07, 6.03, this->FoV));
		
		intervals.push_back(0.0045);

		positions.push_back(vec3(-7.5, 3.29, -0.46));
		angles.push_back(vec3(-5.36, 6.03, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-2.18, 2.64, -1.09));
		angles.push_back(vec3(-6.18, 6.09, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-2.9, 1.8, 0.82));
		angles.push_back(vec3(-6.16, 6.74, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-2.722158, 2.246655, 3.824823));
		angles.push_back(vec3(-5.95, 7.325057, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-3.226718, 3.176694, 6.013127));
		angles.push_back(vec3(-4.775071, 5.969983, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-5.344382, 2.885156, 4.831669));
		angles.push_back(vec3(-4.875073, 6.214976, this->FoV));

		//last position only for interpolation
		positions.push_back(vec3(-5.558214, 2.024369, 3.861058));
		angles.push_back(vec3(-5.515075, 6.139987, this->FoV));


		addCut();

		//Ant Sitting Still

		positions.push_back(vec3(-3.192017, 0.961560, 2.891199));
		angles.push_back(vec3(-4.915076, 6.424987, this->FoV));


		positions.push_back(vec3(-3.192017, 0.961560, 2.891199));
		angles.push_back(vec3(-4.915076, 6.424987, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-3.192017, 0.961560, 2.891199));
		angles.push_back(vec3(-4.915076, 6.424987, this->FoV));
		

		positions.push_back(vec3(-3.192017, 0.961560, 2.891199));
		angles.push_back(vec3(-4.915076, 6.424987, this->FoV));


		addCut();

		//Ant Sitting Still 2

		positions.push_back(vec3(-5.558214, 2.024369, 3.861058));
		angles.push_back(vec3(-5.515075, 6.139987, this->FoV));
		

		positions.push_back(vec3(-5.558214, 2.024369, 3.861058));
		angles.push_back(vec3(-5.515075, 6.139987, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-5.558214, 2.024369, 3.861058));
		angles.push_back(vec3(-5.515075, 6.139987, this->FoV));


		positions.push_back(vec3(-5.558214, 2.024369, 3.861058));
		angles.push_back(vec3(-5.515075, 6.139987, this->FoV));
		

		addCut();

		//Pan around the fire

		positions.push_back(vec3(-0.344633, -0.683054, 3.762594));
		angles.push_back(vec3(-7.780073, 7.144987, this->FoV));


		positions.push_back(vec3(-0.344633, -0.683054, 3.762594));
		angles.push_back(vec3(-7.780073, 7.144987, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-0.054343, 2.751772, 3.322370));
		angles.push_back(vec3(-7.710071, 6.629993, this->FoV));
		
		intervals.push_back(0.0065);

		positions.push_back(vec3(-2.704219, 4.743681, 2.261542));
		angles.push_back(vec3(-6.625063, 5.749993, this->FoV));
		
		intervals.push_back(0.008);

		positions.push_back(vec3(-4.908272, 3.708614, 3.949534));
		angles.push_back(vec3(-5.745038, 5.839998, this->FoV));
		
		intervals.push_back(0.009);

		positions.push_back(vec3(-4.247889, 3.058974, 5.797277));
		angles.push_back(vec3(-4.970037, 5.960000, this->FoV));
		
		intervals.push_back(0.009);

		positions.push_back(vec3(-2.543034, 2.514938, 6.812670));
		angles.push_back(vec3(-3.935034, 6.125004, this->FoV));
		
		intervals.push_back(0.008);
		
		positions.push_back(vec3(-0.638485, 2.353522, 6.067216));
		angles.push_back(vec3(-3.045032, 6.195005, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(0.926948, 1.256024, 2.733205));
		angles.push_back(vec3(-1.470020, 6.375004, this->FoV));
		
		intervals.push_back(0.004);

		positions.push_back(vec3(-3.271664, 1.331918, -1.151470));
		angles.push_back(vec3(-0.335021, 6.345004, this->FoV));
		
		intervals.push_back(0.004);

		positions.push_back(vec3(-6.928247, 1.292648, -2.209969));
		angles.push_back(vec3(0.489979, 6.355004, this->FoV));
		
		intervals.push_back(0.0055);

		positions.push_back(vec3(-5.627995, 1.212076, -4.013000));
		angles.push_back(vec3(0.384979, 6.390005, this->FoV));
		
		intervals.push_back(0.006);

		positions.push_back(vec3(-3.232066, 1.916432, -1.213088));
		angles.push_back(vec3(0.184974, 6.070035, this->FoV));
		
		intervals.push_back(0.006);

		positions.push_back(vec3(-3.129624, 1.255944, 2.321906));
		angles.push_back(vec3(0.259974, 5.980045, this->FoV));
		
		intervals.push_back(0.006);

		positions.push_back(vec3(-3.129624, 1.255944, 2.321906));
		angles.push_back(vec3(0.194974, 6.65, this->FoV));
		
		intervals.push_back(0.0085);

		positions.push_back(vec3(-3.129624, 1.255944, 2.321906));
		angles.push_back(vec3(0.194974, 7.100070, this->FoV));


		positions.push_back(vec3(-3.129624, 1.255944, 2.321906));
		angles.push_back(vec3(0.194974, 7.100070, this->FoV));
		
		
		addCut();

		//Going upwards from the fire

		positions.push_back(vec3(-2.582639, 1.009217, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));


		positions.push_back(vec3(-2.582639, 2.509217, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));
		
		intervals.push_back(0.004);

		positions.push_back(vec3(-2.582639, 3.259217, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));
		
		intervals.push_back(0.004);

		positions.push_back(vec3(-2.582639, 4.009217, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-2.582639, 7.097607, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));
		
		intervals.push_back(0.005);

		positions.push_back(vec3(-2.582639, 14.597607, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));
		
		intervals.push_back(0.004);

		positions.push_back(vec3(-2.582639, 29.597607, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));
		
		intervals.push_back(0.004);

		positions.push_back(vec3(-2.582639, 35, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));


		positions.push_back(vec3(-2.582639, 35, 4.647204));
		angles.push_back(vec3(-2.375022, 4.760033, this->FoV));


		addCut();
		currentInterpPos = 0;

	}
}

bool Camera::updateCamera(const float dt) {

	float dtscalar = 50;
	bool cameraPathFinished = false;

	// Interpolate to next position
	this->position = interpPositions[int(currentInterpPos)];
	this->horizontalAngle = interpAngles[int(currentInterpPos)].x;
	this->verticalAngle = interpAngles[int(currentInterpPos)].y;
	this->FoV = interpAngles[int(currentInterpPos)].z;
	currentInterpPos += (dt * dtscalar);

	if (currentInterpPos >= interpPositions.size()) { cameraPathFinished = true; } //this ends the program after this frame

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
	ProjectionMatrix = glm::perspective(glm::radians(this->FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		this->position,      // Camera is here
		this->position + direction,		// and looks here : at the same position, plus "direction"
		up					// Head is up (set to 0,-1,0 to look upside-down)
	);

	return cameraPathFinished;
}


void Camera::computeMatricesFromInputs(const float dt) {

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Compute new orientation based on mouse
	if (!initialMousePosition) { //see comment in global variable
		this->horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		this->verticalAngle += mouseSpeed * float(768 / 2 - ypos);
	}
	else { initialMousePosition = false; }
	
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(this->verticalAngle) * sin(this->horizontalAngle),
		sin(this->verticalAngle),
		cos(this->verticalAngle) * cos(this->horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(this->horizontalAngle - 3.14f / 2.0f),
		0,
		cos(this->horizontalAngle - 3.14f / 2.0f)
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
			this->position += direction * dt * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			this->position -= direction * dt * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			this->position += right * dt * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			this->position -= right * dt * speed;
		}


		//update Position/Direction depending on user controlled camera
		finalPosition = position;
		finalDirection = position + direction;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (!this->Cpressed) {
			this->Cpressed = true;
			printf("\n");
			printf("positions.push_back(vec3(%f", this->position.x);
			printf(", %f", this->position.y);
			printf(", %f", this->position.z);
			printf("));\n");
			printf("angles.push_back(vec3(%f", this->horizontalAngle);
			printf(", %f", this->verticalAngle);
			printf(", this->FoV));\n");
			printf("intervals.push_back(0.005);\n");
		}
	}

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		this->Cpressed = false;
	}


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(this->FoV), 4.0f / 3.0f, 0.1f, 100.0f);
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

void Camera::addInterpAngles(std::vector<vec3> interpolations) {
	this->interpAngles.insert(this->interpAngles.end(), interpolations.begin(), interpolations.end());
}



// Blended Parabola Interpolation
// Follwoing code from Assignment 2 of Computeranimation in SS2020 by Jonas Prohaska

std::vector<vec3> Camera::GetParabolaInterpolation(std::vector<vec3> positions, std::vector<float> intervals)
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