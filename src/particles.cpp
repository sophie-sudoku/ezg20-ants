#include <vector>
#include <string>
#include <stdio.h>
#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "particles.hpp"



ParticleSystem::ParticleSystem(
    GLuint& programID,
    unsigned int maxParticles
)
{

    // Set global variables
    this->spawnPosition = vec3(
        -2.58,
         0.46,
         4.49
    );
    this->spawnRadius = 1.2;


    // Setup shaders
    this->programID = programID;
    this->ViewMatrix = glGetUniformLocation(programID, "V");
    this->ProjectionMatrix = glGetUniformLocation(programID, "P");


    // Create Particle vertices, generate & fill buffer
    static const GLfloat particle_vertex_buffer_data[] = {
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f
    };

    glGenBuffers(1, &this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_vertex_buffer_data), particle_vertex_buffer_data, GL_STATIC_DRAW);


    // Create Particles, generate & fill position buffer
    this->particles.resize(maxParticles);
    particle_position_buffer_data = new float[maxParticles * 4];

    for (uint i = 0; i < maxParticles; ++i)
    {
        SpawnParticle(i);
        UpdatePositionBuffer(i);
    }

    glGenBuffers(1, &this->positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float), particle_position_buffer_data, GL_DYNAMIC_DRAW);


    // Go through update a couple times so it looks natural on start
    for (unsigned i = 0; i < 200; i++) {
        Update(0.01);
    }

}



ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &vertexBuffer);
    delete particle_position_buffer_data;
}




void ParticleSystem::Draw(
    glm::mat4 ProjectionMatrix,
    glm::mat4 ViewMatrix
)
{
    // Use Particle Program
    glUseProgram(this->programID);
    glUniformMatrix4fv(this->ViewMatrix, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(this->ProjectionMatrix, 1, GL_FALSE, &ProjectionMatrix[0][0]);

    // Update Position Buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->particles.size() * 4 * sizeof(float), this->particle_position_buffer_data);

    // Buffer Vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Buffer positions
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(1, 1);

    // Draw
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->particles.size()); // 3 indices starting at 0 -> 1 triangle

    // Resets & Disables
    glDisable(GL_BLEND);
    glVertexAttribDivisor(1, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}



void ParticleSystem::Update(const float dt)
{

    const float dtscalar = 1.5; //speed adjustment of flame animation

    for (uint i = 0; i < this->particles.size(); ++i)
    {
        // subtract from the particles lifetime
        this->particles[i].lifetime -= dt * dtscalar/2;

        // if the lifetime is below 0 respawn the particle
        if (this->particles[i].lifetime <= 0.0f)
        {
            SpawnParticle(i);
        }

        // move the particle up depending on the delta time
        this->particles[i].position += dt * dtscalar * this->particles[i].velocity;
        this->particles[i].velocity.y += dt * dtscalar * 0.001;

        // update the position buffer
        UpdatePositionBuffer(i);
    }
}



void ParticleSystem::SpawnParticle(unsigned int particleID) {
    // give every particle a random position in a XZ circle around the spawn pos using pythagorean
    float adjacent = RandomNumber(-spawnRadius, spawnRadius);
    float oppositeMax = sqrt(spawnRadius * spawnRadius - adjacent * adjacent);
    float opposite = RandomNumber(-oppositeMax, oppositeMax);

    // add adjacent/opposite to x/z based on coinflip
    if (rand() % 2) {
        this->particles[particleID].position = vec3(
            spawnPosition.x + adjacent,
            spawnPosition.y + RandomNumber(-0.4 * (rand()%2), 0.5), // * rand()/2 makes every second particle start at 0
            spawnPosition.z + opposite
        );
    }
    else {
        this->particles[particleID].position = vec3(
            spawnPosition.x + opposite,
            spawnPosition.y + RandomNumber(-0.3 * (rand() % 2), 0.5),
            spawnPosition.z + adjacent
        );
    }

    // give every particle a lifetime based on distance to center
    float distanceToEdge = spawnRadius - sqrt(opposite * opposite + adjacent * adjacent);
    this->particles[particleID].lifetime = RandomNumber(distanceToEdge, distanceToEdge * 2.0f + 0.8);
    

    // give every particle a random velocity using 2^x to get a few outliers
    // determine random negatives
    int xmult = 1;
    int zmult = 1;
    if (rand() % 2) {
        xmult = -1;
    }
    if (rand() % 2) {
        zmult = -1;
    }
    // 2^8 = 128 / 400 gives range from 1/400 to about 1/4
    this->particles[particleID].velocity = vec3(
        xmult * pow(2,RandomNumber(0,8)) / 400,
        RandomNumber(-0.15,1.52),
        zmult * pow(2, RandomNumber(0,8)) / 400
    );
}



void ParticleSystem::UpdatePositionBuffer(unsigned int particleID) {
    this->particle_position_buffer_data[particleID * 4 + 0] = this->particles[particleID].position[0];
    this->particle_position_buffer_data[particleID * 4 + 1] = this->particles[particleID].position[1];
    this->particle_position_buffer_data[particleID * 4 + 2] = this->particles[particleID].position[2];
    this->particle_position_buffer_data[particleID * 4 + 3] = this->particles[particleID].lifetime;
}


float ParticleSystem::RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}