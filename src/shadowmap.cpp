#include <vector>
#include <string>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL2.h>

#include "shadowmap.hpp"
#include "shader.hpp"
#include "mesh.hpp"

#include <glm/gtx/string_cast.hpp>

Shadowmap::Shadowmap(
)
{
	VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	
	depthProgram = LoadShaders("assets/shader/DepthShader.vert", "assets/shader/DepthShader.frag", "assets/shader/DepthShader.geom");

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	FramebufferName;
	glGenFramebuffers(1, &FramebufferName);
	depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shadowmap::~Shadowmap() {
	glDeleteProgram(depthProgram);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(1, &depthCubemap);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void Shadowmap::DrawSetup() {
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(depthProgram);
}

void Shadowmap::DrawTeardown() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1024, 768);
}

void Shadowmap::Draw(
	Mesh * mesh,
	glm::vec3 lightPos
)
{
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);

	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "shadowMatrix1"), 1, GL_FALSE, &(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "shadowMatrix2"), 1, GL_FALSE, &(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "shadowMatrix3"), 1, GL_FALSE, &(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "shadowMatrix4"), 1, GL_FALSE, &(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "shadowMatrix5"), 1, GL_FALSE, &(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "shadowMatrix6"), 1, GL_FALSE, &(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "model"), 1, GL_FALSE, &mesh->ModelMatrix[0][0]);
	glUniform1f(glGetUniformLocation(depthProgram, "far_plane"), far_plane);
	glUniform3f(glGetUniformLocation(depthProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexbuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->elementbuffer);

	glDrawElements(
		GL_TRIANGLES,
		mesh->indices.size(),
		GL_UNSIGNED_SHORT,
		(void*)0
	);

	glDisableVertexAttribArray(0);
}
