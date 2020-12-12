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
GLboolean enable_gama_correction = true;
GLboolean enable_post_effect = false;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    mainCamera.mouse_callback(window,xpos,ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
   mainCamera.key_callback(window,key,scancode,action,mode);
    if(key == GLFW_KEY_T && action == GLFW_PRESS)
        enable_gama_correction = !enable_gama_correction;
    if(key == GLFW_KEY_Z && action == GLFW_PRESS)
        enable_post_effect = !enable_post_effect;
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

GLuint ConfiguratePostEffect( int width,int height,GLuint &frameBuffer, GLuint  &textureColorBuffer){
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLfloat screenVertexes[] = {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    GLuint screen_array, screenVBO;
    glGenVertexArrays(1, &screen_array);
    glGenBuffers(1, &screenVBO);
    glBindVertexArray(screen_array);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertexes), &screenVertexes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    return screen_array;
}

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
    GLFWwindow* window = glfwCreateWindow(1000, 900, "Mordvincev", nullptr, nullptr);
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




    MyShader shader("../vertex_shader.vs", "../fragment_shader_light_source.fs");
    Model lightCube(shader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "../light.png", width, height, true);

    MyShader screenShader("../vertex_shader_post_effect.vs","../fragment_shader_post_effect.fs" );

    const char *cubeMapPath[6];
    cubeMapPath[1] = "../skybox/hot_bk.png";
    cubeMapPath[3] = "../skybox/hot_dn.png";//correct
    cubeMapPath[0] = "../skybox/hot_ft.png";
    cubeMapPath[5] = "../skybox/hot_lf.png";
    cubeMapPath[4] = "../skybox/hot_rt.png";
    cubeMapPath[2] = "../skybox/hot_up.png";//correct
    auto cubeMap = MyShader::loadCubemap(cubeMapPath);

    MyShader skyboxShader("../vertex_shader_skybox.vs", "../fragment_shader_skybox.fs");
    Model skyBoxCube(skyboxShader, mainCamera, cube_vertices, sizeof(cube_vertices), 36, "", width, height, true);

    MyShader paralaxShader("../vertex_shader_paralax.vs", "../fragment_shader_paralax.fs");
    auto paralaxPlane = Model::GeneratePlaneForParalax(paralaxShader, mainCamera, "../paralax/bricks2.jpg", width, height);
    paralaxPlane.LoadNormalTexture("../paralax/bricks2_normal.jpg");
    paralaxPlane.LoadDephTexture("../paralax/bricks2_disp.jpg");

    Model nPlane = Model::GeneratePlaneForParalax(paralaxShader, mainCamera, "../brickwall.jpg", width, height);
    nPlane.LoadNormalTexture("../brickwall_normal.jpg");

    glEnable(GL_DEPTH_TEST);
    glm::vec3 lightSource(0, 2, 0);
    glm::vec3 lightColor(1,1,1);
    GLuint  frameBufer, screenImage;
    auto quadVAO = ConfiguratePostEffect(width,height,frameBufer, screenImage);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufer);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.2, 0.3, 0.35, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //wireframe для отладки
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glDepthMask(GL_FALSE);
        skyBoxCube.ApplyShader();
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        skyBoxCube.ApplyTransformation(glm::vec3(0,0,0));
        skyBoxCube.IgnoreCamPosition();
        skyBoxCube.Show();
        glDepthMask(GL_TRUE);

        lightCube.ApplyShader();
        lightCube.AddLight(lightColor, lightSource);
        lightCube.ApplyTransformation(lightSource);
        lightCube.Show();

        sCube.ApplyShader();
        sCube.ApplySpectacularTexture();
        sCube.ApplyTransformation(glm::vec3(0,0,-6));
        sCube.AddLight(lightColor, lightSource);
        sCube.ApplyLightParameters();
        sCube.Show();



        nPlane.ApplyShader();
        nPlane.ApplyNormalTexture();
        nPlane.ApplyTransformation(glm::vec3(0,0,-2));
        paralaxShader.SetFloat("heightScale",0);
        //nCube.ApplyRotation(glm::normalize,90.0f(glm::vec3(1.0, 0.0, 1.0)),(float)glfwGetTime() * -10.0f);
        nPlane.AddLight(lightColor, lightSource);
        nPlane.ApplyLightParameters();
        nPlane.Show();



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
        plane.ApplyLightParameters();
        //plane.ApplyLightParameters(glm::vec3(1,0.5f,0.31f), glm::vec3(1,0.5f,0.31f), glm::vec3(0.6,0.6,0.6),64);
        plane.Show();


        paralaxPlane.ApplyShader();
        paralaxShader.SetFloat("heightScale",0.2f);
        paralaxPlane.ApplyNormalTexture();
        paralaxPlane.ApplyDephTexture();
        paralaxPlane.ApplyTransformation(glm::vec3(2,0,-4));
        paralaxPlane.ApplyRotation(glm::vec3(0,0,1),(GLfloat)glfwGetTime()*10.0f);
        paralaxPlane.AddLight(lightColor,lightSource);
        paralaxPlane.ApplyLightParameters();
        paralaxPlane.Show();

        shader.Apply();
        shader.SetBool("enableGama",enable_gama_correction);
        lightedShader.Apply();
        lightedShader.SetBool("enableGama",enable_gama_correction);
        spectacularShader.Apply();
        spectacularShader.SetBool("enableGama",enable_gama_correction);
        paralaxShader.Apply();
        paralaxShader.SetBool("enableGama",enable_gama_correction);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.Apply();
        screenShader.SetBool("enablePostEffect", enable_post_effect);
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenImage);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
