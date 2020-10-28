#ifndef MESH_H
#define	MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <vector>

class Mesh
{
public:
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    GLuint Texture;
    GLuint TextureID;
    GLuint LightID;
    GLuint programID;
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    Mesh(const char* path);
    ~Mesh();
    void SetTexture(const char* path, GLuint& programID);
    void SetupMesh(
        GLuint& vertexbuffer,
        GLuint& uvbuffer,
        GLuint& normalbuffer,
        GLuint& elementbuffer
    );
    void Draw(
        GLuint& vertexbuffer,
        GLuint& uvbuffer,
        GLuint& normalbuffer,
        GLuint& elementbuffer,
        glm::mat4 ProjectionMatrix,
        glm::mat4 ViewMatrix,
        GLuint& MatrixID,
        GLuint& ViewMatrixID,
        GLuint& ModelMatrixID
    );
    void SetTransform(
        glm::mat4 transform
    );

private:
};

#endif

