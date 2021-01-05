#version 330 core

in float lifetime;

out vec3 color;

void main() {
   color = vec3(lifetime, //red, stays the longest and in the end turns black
   lifetime -0.7,	//makes it yellow, then orange, then red
   lifetime - 2.0); //makes it white, decreases the fastest
}