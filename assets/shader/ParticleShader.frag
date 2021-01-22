#version 330 core

in float lifetime;

out vec4 color;

void main() {
   color = vec4(lifetime, //makes it red, then turns black
   lifetime -0.7,		//makes it yellow, then orange, then red
   0,
   1);
}