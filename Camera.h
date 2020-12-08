//
// Created by михаил on 08.12.2020.
//

#ifndef MASH_GRAPH_CAMERA_H
#define MASH_GRAPH_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>


class Camera {
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    bool keys[1024];
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    GLfloat lastX = 400, lastY = 300;
    GLfloat yaw   = -90.0f;
    GLfloat pitch = 0.0f;
public:
    Camera();
    glm::mat4 CameraView();
    void do_movement();
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};


#endif //MASH_GRAPH_CAMERA_H
