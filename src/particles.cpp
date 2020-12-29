#include <vector>
#include <string>
#include <stdio.h>
#include <string>
#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "particles.hpp"



ParticleSystem::ParticleSystem(
    unsigned int maxParticles,
    unsigned int newParticles,
    GLuint& programID
)
{
    this->maxParticles = maxParticles;
    this->newParticles = newParticles;
    this->programID = programID;

    OffsetID = glGetUniformLocation(programID, "offset");
    ColorID = glGetUniformLocation(programID, "color");


    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);


    for (unsigned int i = 0; i < maxParticles; ++i)
        this->particles.push_back(Particle());
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::DrawParticles() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glUseProgram(programID);
    for (Particle p : particles)
    {
        if (p.life > 0.0f)
        {
            glUniform2f(OffsetID, p.pos.x, p.pos.y);
            glUniform4f(ColorID, p.color.x, p.color.y, p.color.z, p.color.w);
            
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleSystem::UpdateParticles()
{
    float dt = 0.1;

    // add new particles
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        unsigned int unusedParticle = getFirstUnusedParticle();
        RespawnParticle(particles[unusedParticle], vec2(0,0));
    }

    // update all particles
    for (unsigned int i = 0; i < maxParticles; ++i)
    {
        Particle& p = particles[i];
        p.life -= dt; // reduce life
        if (p.life > 0.0f)
        {	// particle is alive, thus update
            p.pos -= p.velocity * dt;
            p.color.a -= dt * 2.5f;
        }
    }

}

unsigned int ParticleSystem::getFirstUnusedParticle() {
    // search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < maxParticles; ++i) {
        if (particles[i].life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (particles[i].life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // override first particle if all others are alive
    lastUsedParticle = 0;
    return 0;
}


void ParticleSystem::RespawnParticle(
    Particle p, 
    vec2 offset
) 
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    p.pos = vec2(0,0) + random + offset;
    p.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    p.life = 1.0f;
    p.velocity = vec2(0,1) * 0.1f;
}