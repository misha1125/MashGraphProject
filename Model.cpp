//
// Created by михаил on 08.12.2020.
//


#include "Model.h"

// пока без индексов
Model::Model(MyShader &shader, Camera &camera, GLfloat *vertex, size_t vertex_size, size_t vertex_cnt, const char *image_path,GLfloat width, GLfloat height):
        shader(shader), camera(camera), vertex(vertex), vertex_size(vertex_size), vertex_cnt(vertex_cnt), width(width), height(height)  {

    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    //glGenBuffers(1, &index_buffer);

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertex, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width_im, height_im;
    unsigned char* image = SOIL_load_image(image_path, &width_im, &height_im, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_im, height_im, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout<<"Created sucsessfully "<< vertex_array<<" "<<texture<<" "<<shader.Program<<"\n";
}


void Model::ApplyTransformation(glm::vec3 position) {
    model = glm::mat4(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    view = glm::translate(view, position);
    projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
    GLint projLoc = glGetUniformLocation(shader.Program, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLint camLoc = glGetUniformLocation(shader.Program, "cameraView");
    glUniformMatrix4fv(camLoc, 1, GL_FALSE, glm::value_ptr(camera.CameraView()));
}



void Model::Show() {

    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, vertex_cnt);
    glBindVertexArray(0);
}

void Model::ApplyRotation(glm::vec3 axis, GLfloat angle) {
    model = glm::rotate(model, glm::radians(angle), axis);
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Model::ApplyScale(glm::vec3 scale) {
    model = glm::scale(model,scale);
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Model::AddLight(glm::vec3 lightColor) {
    GLint lightLoc = glGetUniformLocation(shader.Program, "lightColor");
    glUniform3f(lightLoc, lightColor.x, lightColor.y, lightColor.z);
}

void Model::ApplyShader() {
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.Apply();
}
