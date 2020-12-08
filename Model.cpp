//
// Created by михаил on 08.12.2020.
//


#include "Model.h"

// пока без индексов
Model::Model(MyShader &shader, GLfloat *vertex, size_t vertex_size, size_t vertex_cnt, const char *image_path):
        shader(shader), vertex(vertex), vertex_size(vertex_size), vertex_cnt(vertex_cnt) {


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
    int width, height;
    unsigned char* image = SOIL_load_image(image_path, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout<<"Created sucsessfully "<<texture<<"\n";
}

void Model::ApplyTransformation() {

}

void Model::Show() {
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.Apply();
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
