#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
using namespace glm;


class Camera {
private:
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;


	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;

	std::vector<glm::vec3> interpPositions;
	int currentInterpPos;

	//Old, user controlled variables
	float speed;
	float mouseSpeed;
	bool orbit;
	float orbitRadius;
	bool F_currently_pressed;
	bool initialMousePosition;
	vec3 finalPosition;
	vec3 finalDirection;

public:
	void setupCamera(bool userControl);
	void updateCamera(float dt);
	void computeMatricesFromInputs(float dt);
	mat4 getViewMatrix();
	mat4 getProjectionMatrix();

private:
	void addInterpPositions(std::vector<vec3> interpolations);
	std::vector<vec3> GetParabolaInterpSpline(std::vector<vec3> positions, std::vector<float> intervals);
	mat4 ConstructParabolaB(vec3 p1, vec3 p2, vec3 p3, vec3 p4);
	vec4 multiply1x4by4x4(vec4 a, glm::mat4 b);
	mat4 multiply4x4by4x4(glm::mat4 a, glm::mat4 b);
};

#endif