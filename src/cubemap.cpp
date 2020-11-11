#include <vector>
#include <string>
#include <stdio.h>
#include <string>
#include <GL/glew.h>

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
    this->MatrixID = glGetUniformLocation(programID, "MVP");
    this->ViewMatrixID = glGetUniformLocation(programID, "V");
    this->ModelMatrixID = glGetUniformLocation(programID, "M");
    LoadCubemap(cubeMapName, size);
}

void Cubemap::LoadCubemap(const string cubeMapName, const float size)
{
    GLfloat vertices[] = {
        size,  size,  size,
        size, -size,  size,
        size,  size, -size,
        size, -size, -size,
       -size, -size, -size,
       -size,  size, -size,
       -size, -size,  size,
       -size,  size,  size
    };

    GLuint indices[] = {
        0, 1, 3,
        3, 2, 0,
        0, 1, 7,
        7, 6, 1,
        1, 3, 6,
        6, 4, 3,
        3, 2, 4,
        4, 2, 5,
        5, 4, 6,
        6, 5, 7,
        7, 5, 2,
        2, 0, 7
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBOvertices);
    glGenBuffers(1, &VBOindices);

    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

    Texture = SOIL_load_OGL_cubemap(
        (string("assets/textures/") + cubeMapName + string("/px.png")).c_str(), (string("assets/textures/") + cubeMapName + string("/nx.png")).c_str(),
        (string("assets/textures/") + cubeMapName + string("/py.png")).c_str(), (string("assets/textures/") + cubeMapName + string("/ny.png")).c_str(),
        (string("assets/textures/") + cubeMapName + string("/pz.png")).c_str(), (string("assets/textures/") + cubeMapName + string("/nz.png")).c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBOvertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cubemap::Draw(
    glm::mat4 ProjectionMatrix,
    glm::mat4 ViewMatrix
)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glUseProgram(ProgramID);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
    //
    glUniform1i(glGetUniformLocation(ProgramID, "skybox"), 0);
    //glUniform1i(TextureID, 0);

    //glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_SHORT, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}
