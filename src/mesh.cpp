#include <vector>
#include <string>
#include <stdio.h>
#include <string>
#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL2.h>

#include "mesh.hpp"
#include "objloader.hpp"

Mesh::Mesh(
    const char * path
)
{
    loadAssImp(path, indices, indexed_vertices, indexed_uvs, indexed_normals);
}


Mesh::~Mesh() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

void Mesh::SetupMesh()
{
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}

void Mesh::SetShader(
	GLuint& programID,
	const char* path = "",
	glm::vec3& color = glm::vec3(0.0f, 0.0f, 0.0f)
)
{
	this->programID = programID;
	this->MatrixID = glGetUniformLocation(programID, "MVP");
	this->ViewMatrixID = glGetUniformLocation(programID, "V");
	this->ModelMatrixID = glGetUniformLocation(programID, "M");
	this->Color = color;
	if (path != "") {
		Texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture 
		(
			path,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
		TextureID = glGetUniformLocation(programID, "myTextureSampler");
		UseTexture = true;
	}
	ColorID = glGetUniformLocation(programID, "myColor");
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	UseTextureID = glGetUniformLocation(programID, "useTexture");
}

void Mesh::SetTransform(
	glm::mat4 transform
)
{
	ModelMatrix = transform;
}

void Mesh::Draw(
	glm::mat4 ProjectionMatrix,
	glm::mat4 ViewMatrix,
	glm::vec3 lightPos
)
{
	glUseProgram(programID);

	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"
	glUniform3f(ColorID, Color.x, Color.y, Color.z);
	glUniform1i(UseTextureID, UseTexture);
	glm::mat4 MVP1 = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);


	if (Texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);
	}

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
}
