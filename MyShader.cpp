//
// Created by михаил on 07.12.2020.
//

#include <SOIL.h>
#include "MyShader.h"



MyShader::MyShader(const GLchar *vertexPath, const GLchar *fragmentPath) {
    const GLchar* vertex_shader = nullptr;
    const GLchar* fragment_shader = nullptr;
    std::ifstream vertex_file;
    std::ifstream fragment_file;
    std::string v_val;
    std::string f_val;
    try
    {
        vertex_file.open(vertexPath);
        fragment_file.open(fragmentPath);
        std::stringstream v_stream, f_stream;
        v_stream << vertex_file.rdbuf();
        f_stream << fragment_file.rdbuf();
        vertex_file.close();
        fragment_file.close();
        v_val = v_stream.str();
        f_val = f_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "Error reading file" << e.what() << std::endl;
    }
    vertex_shader = v_val.c_str();
    fragment_shader = f_val.c_str();
    GLuint vertex, fragment;
    ApplyCodeToShader(vertex_shader, vertex, GL_VERTEX_SHADER);
    ApplyCodeToShader(fragment_shader, fragment, GL_FRAGMENT_SHADER);
    ApplyShadersToProgram(vertex, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void MyShader::ApplyShadersToProgram(GLuint vertex, GLuint fragment) {
    GLint success;
    Program = glCreateProgram();
    glAttachShader(Program, vertex);
    glAttachShader(Program, fragment);
    glLinkProgram(Program);

    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    GLchar infoLog[512];
    if(!success)
    {
        glGetProgramInfoLog(Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void MyShader::ApplyCodeToShader(const GLchar *shader_code, GLuint &shader_id, GLuint type){
    shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);
    PrintErrorIfHas(shader_id);
}

void MyShader::PrintErrorIfHas(GLuint shader_id) const {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
}

void MyShader::Apply() {
    glUseProgram(this->Program);
}

void MyShader::SetVector3(const char *name, glm::vec3 vec) {
    GLint vecLoc = glGetUniformLocation(Program, name);
    glUniform3f(vecLoc, vec.x, vec.y, vec.z);
}

void MyShader::SetFloat(const char *name, GLfloat val) {
    GLint viewPosLoc = glGetUniformLocation(Program, name);
    glUniform1f(viewPosLoc, val);
}

void MyShader::SetInt(const char *name, GLint val) {
    GLint viewPosLoc = glGetUniformLocation(Program, name);
    glUniform1i(viewPosLoc, val);
}

GLuint MyShader::loadCubemap(const char ** paths) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (unsigned int i = 0; i < 6; i++)
    {
        int width_im, height_im;
        unsigned char* data = SOIL_load_image(paths[i], &width_im, &height_im, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB,
                     width_im, height_im, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        SOIL_free_image_data(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureID;
}

void MyShader::SetBool(const char *name, GLboolean val) {
    GLint viewPosLoc = glGetUniformLocation(Program, name);
    glUniform1i(viewPosLoc, val);
}
