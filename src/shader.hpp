#ifndef SHADER_HPP
#define SHADER_HPP

GLuint LoadSingleShader(const char* file_path, unsigned int type);
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path, const char* geometry_file_path);

#endif
