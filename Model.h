//
// Created by михаил on 08.12.2020.
//

#ifndef MASH_GRAPH_MODEL_H
#define MASH_GRAPH_MODEL_H


#include "MyShader.h"
#include "Camera.h"

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {


    MyShader &shader;
    Camera &camera;
    GLfloat *vertex;
    size_t vertex_size, vertex_cnt;
    GLuint texture;
    GLuint spectecularTexture;
    GLuint normalTexture;
    GLuint vertex_buffer, vertex_array;
    GLfloat width, height;
    glm::mat4 model;
    void LoadTexture(const char *path, GLuint &texture);
public:
    Model(MyShader &shader, Camera &camera,  GLfloat *vertex,size_t vertex_size, size_t vertex_cnt, const char *image_path,
          GLfloat width, GLfloat height, bool has_normal);
    void ApplyTransformation(glm::vec3 position);
    void ApplyRotation(glm::vec3 axis, GLfloat angle);
    void ApplyScale(glm::vec3 scale);
    void Show();
    void AddLight(glm::vec3 lightColor, glm::vec3 lightSource);
    void ApplyLightParameters(glm::vec3 ambient = glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
                              glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f), GLfloat shines = 40);
    void ApplyShader();
    void LoadSpectacularTexture(const char* path);
    void ApplySpectacularTexture();
    void LoadNormalTexture(const char* path);
    void ApplyNormalTexture();
    void ChangeTexture(GLuint newTexture);
    void IgnoreCamPosition();
};


#endif //MASH_GRAPH_MODEL_H
