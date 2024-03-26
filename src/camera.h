#ifndef CAMERA_H_
#define CAMERA_H_

#include "constants.h"
#include "window.h"
#include <glm/glm.hpp>

class Camera {
public:
    Camera(Window &w);
    glm::mat4 lookAt();
    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    void printPos();

private:
    Window &window;
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 dir;
    glm::vec3 right;
    double xprev = Constants::WIDTH / 2.0f;
    double yprev = Constants::HEIGHT / 2.0f;
    float yaw = 45.0f;
    float pitch = 0.0f;
    bool first = false;

    // constants
    static constexpr float speed = 0.05f;
    static constexpr int fov = 90;
    static glm::vec3 world_up;
};


#endif  // CAMERA_H_
