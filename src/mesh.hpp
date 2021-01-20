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
private:
    bool UseTexture = false;
public:
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    GLuint Texture;
    GLuint TextureID;
    GLuint DepthTexture;
    GLuint UseDepthTextureID;
    glm::vec3 Color;
    GLuint ColorID;
    GLuint LightID;
    GLuint programID;
    GLuint UseTextureID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint DepthBiasID;
    GLuint DepthTextureID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 depthBiasMVP = glm::mat4(1.0);
    Mesh(const char* path);
    ~Mesh();
    void SetShader(GLuint& programID, const char* path, glm::vec3& color);
    void SetupMesh();
    void Draw(
        glm::mat4 ProjectionMatrix,
        glm::mat4 ViewMatrix,
        glm::vec3 lightPos
    );
    void SetTransform(
        glm::mat4 transform
    );
    void Mesh::SetDepthTexture(
        GLuint& DepthTexture
    );
    void Mesh::SetDepthBiasMVP(
        glm::mat4 depthBiasMVP
    );

private:
};

#endif

