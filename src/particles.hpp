#ifndef PARTICLES_H
#define	PARTICLES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <vector>


struct Particle {
    glm::vec2 pos, velocity;
    glm::vec4 color;
    float     life;

    Particle()
        : pos(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};


class ParticleSystem
{
private:
    GLuint particleTexture;
    GLuint programID;
    GLuint OffsetID;
    GLuint ColorID;

    unsigned int maxParticles;
    unsigned int newParticles;
    std::vector<Particle> particles;
    unsigned int VAO;

    unsigned int lastUsedParticle = 0;

public:
    ParticleSystem(unsigned int maxParticles, unsigned int newParticles, GLuint& particleProgram);
    ~ParticleSystem();
    void DrawParticles();
    void UpdateParticles();
    unsigned int getFirstUnusedParticle();
    void RespawnParticle(Particle p, vec2 offset);

};


#endif