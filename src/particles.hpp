#ifndef PARTICLES_H
#define	PARTICLES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <vector>



class ParticleSystem
{
private:

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        float     lifetime;
    };

    GLuint programID;

    std::vector<Particle> particles;
    vec3 spawnPosition;
    float spawnRadius;

    GLuint vertexBuffer;
    GLuint positionBuffer;
    float* particle_position_buffer_data;

    GLuint ViewMatrix;
    GLuint ProjectionMatrix;

    GLuint ParticleSize;

public:
    ParticleSystem(
        GLuint& particleProgram,
        unsigned int maxParticles
    );
    ~ParticleSystem();
    void Draw(
        glm::mat4 ProjectionMatrix,
        glm::mat4 ViewMatrix
    );
    void Update(const float dt);
    void setUniformParticleSize(const float particleSize);

private:
    void ParticleSystem::SpawnParticle(unsigned int particleID);
    void ParticleSystem::UpdatePosition(unsigned int particleID);
    float RandomNumber(float Min, float Max);

};


#endif