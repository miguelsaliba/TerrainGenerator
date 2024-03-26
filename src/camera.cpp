#include "camera.h"
#include "constants.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

glm::vec3 Camera::world_up = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(Window &w) : pos(glm::vec3(0.0f, 0.0f, 0.0f)),
               up(glm::vec3(0.0f, 1.0f, 0.0f)),
               dir(glm::vec3(0.0f, 0.0f, -1.0f)),
               window(w) {
    right = glm::normalize(glm::cross(dir, up));
}

void Camera::move_forward() {
    pos += speed * dir;
}

void Camera::move_backward() {
    pos -= speed * dir;
}

void Camera::move_right() {
    pos += speed * right;
}

void Camera::move_left() {
    pos -= speed * right;
}

glm::mat4 Camera::lookAt() {
    return glm::lookAt(pos, pos + dir, up);
}

void Camera::printPos() {
    std::cout << "Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ')' << std::endl;
}

void Camera::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (first) {
        xprev = xpos;
        yprev = ypos;
        first = false;
    }
    double xdelta = xpos - xprev;
    double ydelta = ypos - yprev;
    xprev = xpos;
    yprev = ypos;

    xdelta *= Constants::SENSITIVITY;
    ydelta *= Constants::SENSITIVITY;

    yaw += xdelta;
    pitch += ydelta;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 new_dir;
    new_dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_dir.y = sin(glm::radians(pitch));
    new_dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir = glm::normalize(new_dir);
    right = glm::normalize(glm::cross(dir, world_up));
    up = glm::normalize(glm::cross(right, dir));
}
