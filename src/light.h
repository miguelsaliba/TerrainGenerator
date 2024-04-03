#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>

class Light {
public:
    Light();
    void ImGui();
    glm::vec3 &position() { return position_; }
    glm::vec3 &color() { return color_; }

private:
    glm::vec3 position_;
    glm::vec3 color_;
};

#endif  // LIGHT_H_
