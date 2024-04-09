#ifndef CAMERA_H_
#define CAMERA_H_

#include "constants.h"
#include "window.h"
#include <glm/glm.hpp>

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera();
    glm::mat4 lookAt();
    glm::mat4 projection();
    glm::vec3 &position();
    void move(Direction direction, float delta_time);
    void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void printPos();
    void ImGui();

private:
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 dir;
    glm::vec3 right;
    double xprev = Constants::WIDTH / 2.0f;
    double yprev = Constants::HEIGHT / 2.0f;
    float yaw = 45.0f;
    float pitch = 0.0f;
    float near = 0.1f;
    float far = 1000.0f;

    // constants
    float speed = 100.0f;
    float fov = 90.0f;
    static glm::vec3 world_up;
};


#endif  // CAMERA_H_
