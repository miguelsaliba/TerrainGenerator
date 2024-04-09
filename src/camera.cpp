#include "camera.h"
#include "constants.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

glm::vec3 Camera::world_up = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : pos(glm::vec3(50.0f, 10.0f, 50.0f)),
               up(glm::vec3(0.0f, 1.0f, 0.0f)),
               dir(glm::vec3(0.0f, 0.0f, -1.0f)) {
    right = glm::normalize(glm::cross(dir, world_up));
}

glm::mat4 Camera::projection() {
    return glm::perspective(glm::radians(fov), (float) Constants::WIDTH / (float) Constants::HEIGHT, near, far);
}

glm::vec3 &Camera::position() {
    return pos;
}

void Camera::move(Direction direction, float deltaTime) {
    switch (direction) {
        case FORWARD:
            pos += speed * dir * deltaTime;
            break;
        case BACKWARD:
            pos -= speed * dir * deltaTime;
            break;
        case LEFT:
            pos -= speed * right * deltaTime;
            break;
        case RIGHT:
            pos += speed * right * deltaTime;
            break;
        case UP:
            pos += speed * up * deltaTime;
            break;
        case DOWN:
            pos -= speed * up * deltaTime;
            break;
        }
}

glm::mat4 Camera::lookAt() {
    return glm::lookAt(pos, pos + dir, up);
}

void Camera::ImGui() {
    ImGui::InputFloat("Speed", &speed);
    ImGui::InputFloat("FOV", &fov);
    ImGui::InputFloat("Near", &near);
    ImGui::InputFloat("Far", &far);
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
    ImGui::Text("Yaw: %f", yaw);
    ImGui::Text("Pitch: %f", pitch);
}

void Camera::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    double xdelta = xpos - xprev;
    double ydelta = yprev - ypos;
    xprev = xpos;
    yprev = ypos;

    xdelta *= Constants::SENSITIVITY;
    ydelta *= Constants::SENSITIVITY;

    yaw += xdelta;
    pitch += ydelta;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } if (pitch < -89.0f) {
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

void Camera::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    fov -= yoffset * 2;
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 90.0f) {
        fov = 90.0f;
    }
}

void Camera::printPos() {
    std::cout << "Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ')' << std::endl;
}

