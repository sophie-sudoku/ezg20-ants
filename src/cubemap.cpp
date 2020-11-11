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

#include "cubemap.hpp"

Cubemap::Cubemap(
    const string cubeMapName, const float size, GLuint& programID
)
{
    this->ProgramID = programID;
    LoadCubemap(cubeMapName, size);
}

void Cubemap::LoadCubemap(const string cubeMapName, const float size)
{
    ProjectionMatrixID = glGetUniformLocation(ProgramID, "projection");
    ViewMatrixID = glGetUniformLocation(ProgramID, "view");

    static float vertices[] = {
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

    glBindVertexArray(0);

    Texture = SOIL_load_OGL_cubemap(
        (string("assets/textures/") + cubeMapName + string("/px.png")).c_str(), (string("assets/textures/") + cubeMapName + string("/nx.png")).c_str(),
        (string("assets/textures/") + cubeMapName + string("/ny.png")).c_str(), (string("assets/textures/") + cubeMapName + string("/py.png")).c_str(),
        (string("assets/textures/") + cubeMapName + string("/pz.png")).c_str(), (string("assets/textures/") + cubeMapName + string("/nz.png")).c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    TextureID = glGetUniformLocation(ProgramID, "skybox");

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::Draw(
    glm::mat4 ProjectionMatrix,
    glm::mat4 ViewMatrix
)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    
    glm::mat4 viewNew = glm::mat4(glm::mat3(ViewMatrix));

    glUseProgram(ProgramID);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, glm::value_ptr(viewNew));
    glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
    glUniform1i(TextureID, 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}
