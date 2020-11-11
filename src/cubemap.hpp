#ifndef H_SKY
#define H_SKY

#include <vector>
#include <string>
#include <SOIL2.h>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

using std::cerr;
using std::endl;
using std::string;
using std::vector;

class Cubemap {
private:
    unsigned int ProgramID;
    unsigned int TextureID;
    unsigned int Texture;
    GLuint ProjectionMatrixID;
    GLuint ViewMatrixID;
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    void LoadCubemap(const string cubeMapName, const float size);
    GLuint vao;
    GLuint vbo;

public:
    Cubemap(const string cubeMapName, const float size, GLuint& programID);
    void Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
};


#endif