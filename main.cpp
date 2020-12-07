#include <iostream>
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <SOIL.h>
#include "MyShader.h"
#include <string>
#include <sstream>
#include <cstdio>
#include <memory>
#include <cmath>


GLfloat color = 0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true,
    // и приложение после этого закроется
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        color += 0.05;
    }
}

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


void draw_triangle(GLfloat *vertices, GLfloat color, int count_of_points, size_t size_of_ver){
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, size_of_ver, vertices, GL_STATIC_DRAW);
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);//сообщаем особенности хранения буфера
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, count_of_points);
    glBindVertexArray(0);
}

void draw_triangle_with_indexes(GLfloat *vertices, GLuint * indexes, GLfloat color, size_t size_of_ver, size_t size_of_indexes){
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, size_of_ver, vertices, GL_STATIC_DRAW);
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);//сообщаем особенности хранения буфера
    glEnableVertexAttribArray(0);

    GLuint index_buffer; //для индексов
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indexes, indexes, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void draw_different_triangles() {
    GLfloat vertices1[] = {
            // Первый треугольник
            0.5f,  0.5f, 0.0f,  // Верхний правый угол
            0.5f, -0.5f, 0.0f,  // Нижний правый угол
            -0.5f,  0.5f, 1.0f,  // Верхний левый угол
    };
    draw_triangle(vertices1, 0, 3, sizeof(vertices1));
    GLfloat vertices2[] = {
            // Первый треугольник
            0.5f, -0.5f, 0.0f,  // Нижний правый угол
            -0.5f, -0.5f, 0.0f,  // Нижний левый угол
            -0.5f,  0.5f, 0.0f   // Верхний левый угол
    };
    draw_triangle(vertices2, 0.4f, 3, sizeof(vertices2));
}
void draw_textures(){
    GLfloat texCoords[] = {
            0.0f, 0.0f,  // Нижний левый угол
            1.0f, 0.0f,  // Нижний правый угол
            0.5f, 1.0f   // Верхняя центральная сторона
    };
    int width, height;
    unsigned char* image = SOIL_load_image("../texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
}

void draw_different_triangles_indexes() {
    GLfloat vertices[] = {
            0.5f,  0.5f, 0.0f,  // Верхний правый угол
            0.5f, -0.5f, 0.0f,  // Нижний правый угол
            -0.5f, -0.5f, 0.0f,  // Нижний левый угол
            -0.5f,  0.5f, 0.0f   // Верхний левый угол
    };
    GLuint indices1[] = {  // Помните, что мы начинаем с 0!
            0, 1, 3,   // Первый треугольник
    };
    GLuint indices2[] = {  // Помните, что мы начинаем с 0!
            1, 2, 3    // Второй треугольник
    };

    draw_triangle_with_indexes(vertices, indices1, 0.4f, sizeof(vertices), sizeof(indices1));
    draw_triangle_with_indexes(vertices, indices2, 0, sizeof(vertices), sizeof(indices2));
    draw_textures();
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
    glfwSetKeyCallback(window, key_callback);
    glViewport(0, 0, width, height);
    MyShader shader("../vertex_shader.vs", "../fragment_shader.fs");
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // команды отрисовки тут
        glClearColor(0.2, 0.3, 0.35, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //wireframe для отладки
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader.Apply();
        draw_different_triangles_indexes();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}