#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec4 position_lifetime;

uniform mat4 V;
uniform mat4 P;

out float lifetime;

void main(){

    lifetime = position_lifetime.w;
    float particleSize = 0.01;
    if (lifetime > 1) {
        //mostly so white particles shine brighter in the middle of the fire without affecting the others too much
        particleSize = min(0.01 * lifetime, 0.015);
    }

    //View Projection Adjustments
    vec4 position_viewspace = V * vec4( position_lifetime.xyz, 1 );
    position_viewspace.xy += particleSize * (vertex.xy - vec2(0.5));
    gl_Position = P * position_viewspace;
}