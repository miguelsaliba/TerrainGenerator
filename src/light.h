#ifndef LIGHT_H_
#define LIGHT_H_

#include "shader.h"
#include <glm/glm.hpp>
#include <imgui.h>

class Light {
public:
    Light() : position_(glm::vec3(2000.0f, 2600.0f, 100.0f)), color_(glm::vec3(1.0f, 1.0f, 1.0f)) { }
    void ImGui() {
        ImGui::DragFloat3("Position", &position_.x);
        ImGui::ColorEdit3("Color", &color_.x);
        ImGui::SliderFloat("Specular Strength", &specular_strength_, 0.0f, 1.0f);
        ImGui::SliderFloat("Ambient Strength", &ambient_strength_, 0.0f, 1.0f);
    }
    void set_uniforms(Shader &shader) {
        shader.setVec3("lightPos", position_);
        shader.setVec3("lightColor", color_);
        shader.setFloat("specularStrength", specular_strength_);
        shader.setFloat("ambientStrength", ambient_strength_);
    }
    glm::vec3 &position() { return position_; }
    glm::vec3 &color() { return color_; }

private:
    glm::vec3 position_;
    glm::vec3 color_;
    float specular_strength_ = 0.1f;
    float ambient_strength_ = 0.1f;
};

#endif  // LIGHT_H_
