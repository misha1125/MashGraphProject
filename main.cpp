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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};



GLfloat plane_vertices[] = {
        -0.5f, 0, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        0.5f, 0, -0.5f,  1.0f, 1.0f,0.0f,  1.0f,  0.0f,
        0.5f, 0,  0.5f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f,
        0.5f, 0,  0.5f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f,
        -0.5f, 0,  0.5f,  0.0f, 0.0f,0.0f,  1.0f,  0.0f,
        -0.5f, 0, -0.5f,  0.0f, 1.0f,0.0f,  1.0f,  0.0f,
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

    MyShader lightedShader("../vertex_shader.vs", "../fragment_shader_diffuse_map.fs");
    Model cube(lightedShader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../specular.png", width, height, true);
    Model plane(lightedShader, mainCamera, plane_vertices, sizeof(plane_vertices), 6, "../chess.png", width, height, true);

    MyShader spectacularShader("../vertex_shader.vs", "../fragment_shader_spectral_map.fs");
    Model sCube(spectacularShader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../box.png", width, height, true);
    sCube.LoadSpectacularTexture("../spec_box.png");

    MyShader normalAndSpectacularShader("../vertex_shader_normal_map.vs", "../fragment_shader_spectral_and_normal_map.fs");
    Model nCube(normalAndSpectacularShader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../brickwall.jpg", width, height, true);
    nCube.LoadSpectacularTexture("../brickwall_spec.jpg");
    nCube.LoadNormalTexture("../brickwall_normal.jpg");

    MyShader shader("../vertex_shader.vs", "../fragment_shader_light_source.fs");
    Model lightCube(shader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../light.png", width, height, true);

    glEnable(GL_DEPTH_TEST);
    glm::vec3 lightSource(0, 1, -4);
    glm::vec3 lightColor(1,1,1);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2, 0.3, 0.35, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //wireframe для отладки
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //lightColor = glm::vec3(0.5f*sin(glfwGetTime()*0.2),0.5f*sin(glfwGetTime()*0.1),0.5f*sin(glfwGetTime()*0.5));

        lightCube.ApplyShader();
        lightCube.AddLight(lightColor, lightSource);
        lightCube.ApplyTransformation(lightSource);
        lightCube.Show();



        sCube.ApplyShader();
        sCube.ApplySpectacularTexture();
        sCube.ApplyTransformation(glm::vec3(0,0,-1));
        sCube.AddLight(lightColor, lightSource);
        sCube.ApplyLightParameters();
        sCube.Show();

        nCube.ApplyShader();
        nCube.ApplySpectacularTexture();
        nCube.ApplyNormalTexture();
        nCube.ApplyTransformation(glm::vec3(0,0,-2));
        //cube.ApplyRotation(glm::vec3(0,0,1),(GLfloat)glfwGetTime()*20.0f);
        nCube.AddLight(lightColor, lightSource);
        nCube.ApplyLightParameters();
        nCube.Show();


        cube.ApplyShader();
        cube.ApplyTransformation(glm::vec3(-1,-1,-6));
        cube.ApplyRotation(glm::vec3(0,1,1),(GLfloat)glfwGetTime()*50.0f);
        cube.AddLight(lightColor, lightSource);
        cube.ApplyLightParameters();
        cube.Show();

        cube.ApplyShader();
        cube.ApplyTransformation(glm::vec3(4,-2,-10));
        cube.ApplyRotation(glm::vec3(0,1,1),(GLfloat)glfwGetTime()*50.0f);
        cube.AddLight(lightColor, lightSource);
        cube.ApplyLightParameters();
        cube.Show();

        plane.ApplyShader();
        plane.ApplyTransformation(glm::vec3(1,-3,-6));
        plane.ApplyScale(glm::vec3(10,1,10));
        plane.AddLight(lightColor,lightSource);
        plane.ApplyLightParameters(glm::vec3(1,0.5f,0.31f), glm::vec3(1,0.5f,0.31f), glm::vec3(0.6,0.6,0.6),64);
        plane.Show();


        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);

    glfwTerminate();
    return 0;
}
