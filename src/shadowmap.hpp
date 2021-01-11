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
public:
    unsigned int depthCubemap;
    Shadowmap();
    ~Shadowmap();
    void DrawSetup();
    void DrawTeardown();
    void Draw(Mesh * mesh, glm::vec3 lightPos);
};


#endif