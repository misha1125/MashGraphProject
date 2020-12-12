//
// Created by михаил on 08.12.2020.
//


#include "Model.h"

// пока без индексов
Model::Model(MyShader &shader, Camera &camera, GLfloat *vertex, size_t vertex_size, size_t vertex_cnt, const char *image_path,
             GLfloat width, GLfloat height, bool has_normal):
        shader(shader), camera(camera), vertex(vertex), vertex_size(vertex_size), vertex_cnt(vertex_cnt), width(width), height(height)  {

    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    //glGenBuffers(1, &index_buffer);

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertex, GL_STATIC_DRAW);

    if (!has_normal){
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    } else {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
    }

    glBindVertexArray(0);
    LoadTexture(image_path, texture);
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
    //std::cout<<shader.Program<<" "<<camera.cameraPos.x<<" "<<width << " "<<height <<"\n";
}



void Model::Show() {
    //std::cout<<"Here "<<vertex_array<<" "<<vertex_cnt<<"\n";
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

void Model::AddLight(glm::vec3 lightColor, glm::vec3 lightSource) {
    GLint lightLoc = glGetUniformLocation(shader.Program, "lightColor");
    glUniform3f(lightLoc, lightColor.x, lightColor.y, lightColor.z);
    GLint lightPosLoc = glGetUniformLocation(shader.Program, "lightPos");
    glUniform3f(lightPosLoc, lightSource.x, lightSource.y, lightSource.z);
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

}

void Model::ApplyShader() {
    //glActiveTexture(GL_TEXTURE0);
    //std::cout<<"Here "<<shader.Program<<"\n";
    shader.Apply();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


}

void Model::ApplyLightParameters(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shines) {
    shader.SetVector3("material.ambient", ambient);
    shader.SetVector3("material.diffuse", diffuse);
    shader.SetVector3("material.specular", specular);
    shader.SetFloat("material.shininess", shines);

    shader.SetFloat("material.constant",  0.4f);
    shader.SetFloat("material.linear",    0.03f);
    shader.SetFloat("material.quadratic", 0.003f);
}

void Model::LoadSpectacularTexture(const char *path) {
    LoadTexture(path, spectecularTexture);
}

void Model::ApplySpectacularTexture() {
    shader.SetInt("material.specularTex", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spectecularTexture);
}

void Model::LoadNormalTexture(const char *path) {
   LoadTexture(path, normalTexture);
}

void Model::ApplyNormalTexture() {
    shader.SetInt("normalTexture", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
}

void Model::LoadDephTexture(const char *path) {
    LoadTexture(path, dephMap);
}

void Model::ApplyDephTexture() {
    shader.SetInt("depthMap", 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, dephMap);
}

void Model::LoadTexture(const char *path, GLuint &texture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width_im, height_im;
    unsigned char* image = SOIL_load_image(path, &width_im, &height_im, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width_im, height_im, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout<<"Loaded sucsessfully "<< texture<<"\n";
}

void Model::ChangeTexture(GLuint newTexture){
    texture = newTexture;
}

void Model::IgnoreCamPosition(){
    glm::mat4 view = glm::mat4(glm::mat3(camera.CameraView()));
    GLint camLoc = glGetUniformLocation(shader.Program, "cameraView");
    glUniformMatrix4fv(camLoc, 1, GL_FALSE, glm::value_ptr(view));
}


Model Model::GeneratePlaneForParalax(MyShader &shader, Camera &camera, const char *path, GLfloat width, GLfloat height) {
    Model result(shader, camera, path, width, height);
    glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
    glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);


    GLfloat planeVertices[] = {
            // координаты            // нормали      // текст. коорд.  // касательные                         // бикасательные
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    result.vertex = planeVertices;
    result.vertex_size = sizeof(planeVertices);
    result.vertex_cnt = 6;
    GLuint VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
    result.vertex_array = VAO;
    result.vertex_buffer = VBO;
    std::cout<<"Plane created sucsessfully "<< result.vertex_array<<" "<<result.texture<<" "<<" "<<shader.Program<<" "<<result.shader.Program<<"\n";
    return result;
}

Model::Model(MyShader &shader, Camera &camera, const char *image_path, GLfloat width, GLfloat height):
        shader(shader), camera(camera), width(width), height(height) {
    LoadTexture(image_path, texture);
}



