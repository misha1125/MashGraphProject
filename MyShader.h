//
// Created by михаил on 07.12.2020.
//

#ifndef MASH_GRAPH_MYSHADER_H
#define MASH_GRAPH_MYSHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/vec3.hpp>

#define GLEW_STATIC
#include "libs/include/glew.h"
#include "libs/include/glfw3.h"



class MyShader {
public:

    GLuint Program;

    MyShader(const GLchar* vertexPath, const GLchar* fragmentPath);

    void Apply();

    void PrintErrorIfHas(GLuint shader_id) const;

    void ApplyShadersToProgram(GLuint vertex, GLuint fragment);

    void ApplyCodeToShader(const GLchar *shader_code, GLuint &shader_id, GLuint type);
    void SetVector3(const char * name, glm::vec3 vec);
    void SetFloat(const char * name, GLfloat val);
};


#endif //MASH_GRAPH_MYSHADER_H
