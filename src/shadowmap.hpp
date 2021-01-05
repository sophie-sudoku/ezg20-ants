#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <vector>
#include <string>
#include <SOIL2.h>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.hpp"

using namespace glm;

using std::cerr;
using std::endl;
using std::string;
using std::vector;

class Shadowmap {
private:
    GLuint depthProgram;
    GLuint VertexArrayID;
    GLuint FramebufferName;
    // The bias matrix is used to map values from [-1,1] to [0,1],
    // to map depthMVP values to texture sampling values
    glm::mat4 biasMatrix = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );
    glm::mat4 depthMVP = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );
public:
    GLuint depthTexture;
    glm::mat4 depthBiasMVP;
    Shadowmap();
    ~Shadowmap();
    void DrawSetup();
    void DrawTeardown();
    void Draw(Mesh * mesh, glm::vec3 lightPos);
};


#endif