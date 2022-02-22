#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrix1;
uniform mat4 shadowMatrix2;
uniform mat4 shadowMatrix3;
uniform mat4 shadowMatrix4;
uniform mat4 shadowMatrix5;
uniform mat4 shadowMatrix6;

out vec4 FragPos;

void main()
{
    gl_Layer = 0;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrix1 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
    gl_Layer = 1;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrix2 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
    gl_Layer = 2;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrix3 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
    gl_Layer = 3;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrix4 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
        gl_Layer = 4;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrix5 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
    gl_Layer = 5;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrix6 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
}
