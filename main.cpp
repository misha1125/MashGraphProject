#include <iostream>
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <SOIL.h>
#include "MyShader.h"
#include "Model.h"
#include "Camera.h"
#include <string>
#include <sstream>
#include <cstdio>
#include <memory>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLuint vertex_buffer, vertex_array, index_buffer;
Camera mainCamera;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    mainCamera.mouse_callback(window,xpos,ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
   mainCamera.key_callback(window,key,scancode,action,mode);
}



GLfloat cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};



GLfloat plane_vertices[] = {
        -0.5f, 0, -0.5f,  0.0f, 1.0f,
        0.5f, 0, -0.5f,  1.0f, 1.0f,
        0.5f, 0,  0.5f,  1.0f, 0.0f,
        0.5f, 0,  0.5f,  1.0f, 0.0f,
        -0.5f, 0,  0.5f,  0.0f, 0.0f,
        -0.5f, 0, -0.5f,  0.0f, 1.0f,
};


int main()
{
    //Инициализация GLFW
    glfwInit();
    //Настройка GLFW
    //Задается минимальная требуемая версия OpenGL.
    //Мажорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Выключение возможности изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Mordvincev", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    MyShader lightShader("../vertex_shader.vs", "../fragment_shader_light.fs");
    Model cube(lightShader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../specular.png" ,width, height);
    Model plane(lightShader, mainCamera, plane_vertices, sizeof(plane_vertices), 6, "../chess.png" ,width, height);

    MyShader shader("../vertex_shader.vs", "../fragment_shader.fs");
    Model lightCube(shader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../specular.png" ,width, height);

    glEnable(GL_DEPTH_TEST);
    glm::vec3 lightSource(1.2f, 1.0f, 2.0f);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2, 0.3, 0.35, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //wireframe для отладки
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        lightCube.ApplyShader();
        lightCube.ApplyTransformation(glm::vec3(0,0,-2));
        lightCube.Show();

        cube.ApplyShader();
        cube.ApplyTransformation(glm::vec3(0,0,-3));
        cube.ApplyRotation(glm::vec3(0,1,1),(GLfloat)glfwGetTime()*50.0f);
        cube.Show();
        cube.AddLight(glm::vec3(1,1,1));


        cube.ApplyShader();
        cube.ApplyTransformation(glm::vec3(-1,-1,-6));
        cube.ApplyRotation(glm::vec3(0,1,1),(GLfloat)glfwGetTime()*50.0f);
        cube.Show();
        cube.AddLight(glm::vec3(1,1,1));


        cube.ApplyShader();
        cube.ApplyTransformation(glm::vec3(4,-2,-10));
        cube.ApplyRotation(glm::vec3(0,1,1),(GLfloat)glfwGetTime()*50.0f);
        cube.Show();
        cube.AddLight(glm::vec3(1,1,1));


        plane.ApplyShader();
        plane.ApplyTransformation(glm::vec3(1,-3,-6));
        plane.ApplyScale(glm::vec3(10,1,10));
        plane.Show();
        plane.AddLight(glm::vec3(1,1,1));


        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);

    glfwTerminate();
    return 0;
}
