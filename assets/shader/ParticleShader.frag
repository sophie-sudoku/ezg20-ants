#version 330 core

in float lifetime;

out vec3 color;

void main() {
   color = vec3(lifetime, //makes it red, then turns black
   lifetime -0.7,		//makes it yellow, then orange, then red
   0);
}