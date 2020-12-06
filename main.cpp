#include <iostream>
#define GLEW_STATIC
#include "libs/include/glew.h"
#include "libs/include/glfw3.h"
#include <string>
#include <sstream>
#include <cstdio>
#include <memory>

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

void CompiliteShader(GLfloat color){
    const char* vertex_shader = "#version 330 core\n"
            "layout (location = 0) in vec3 position; // Устанавливаем позицию атрибута в 0\n"
            "\n"
            "out vec4 vertexColor; // Передаем цвет во фрагментный шейдер\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(position, 1.0); // Напрямую передаем vec3 в vec4\n"
            "    vertexColor = vec4(position.x, position.y,position.z, 1.0f); // Устанавливаем значение выходной переменной в темно-красный цвет.\n"
            "}";
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    std::string fragment_shader = "#version 330 core\n"
                                    "in vec4 vertexColor; // Входная переменная из вершинного шейдера (то же название и тот же тип)\n"
                                    "\n"
                                    "out vec4 color;\n"
                                    "\n"
                                    "void main()\n"
                                    "{\n"
                                    "    color = vertexColor;\n"
                                    "} ";
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fr = fragment_shader.c_str();
    glShaderSource(fragmentShader, 1, &fr, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::CONNECTION FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void draw_triangle(GLfloat *vertices, GLfloat color, int count_of_points, size_t size_of_ver){
    CompiliteShader(color);
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
    CompiliteShader(color);

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
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // команды отрисовки тут
        glClearColor(0.2, 0.3, 0.35, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //wireframe для отладки
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        draw_different_triangles_indexes();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}