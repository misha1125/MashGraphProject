//
// Created by михаил on 08.12.2020.
//

#ifndef MASH_GRAPH_MODEL_H
#define MASH_GRAPH_MODEL_H


#include "MyShader.h"
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <SOIL.h>

class Model {
public:
    MyShader &shader;
    GLfloat *vertex;
    size_t vertex_size, vertex_cnt;
    GLuint texture;
    GLuint vertex_buffer, vertex_array;

    Model(MyShader &shader, GLfloat *vertex,size_t vertex_size, size_t vertex_cnt, const char *image_path);
    void ApplyTransformation();
    void Show();
};


#endif //MASH_GRAPH_MODEL_H
