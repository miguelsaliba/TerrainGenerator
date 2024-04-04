#include "light.h"
#include <imgui.h>

Light::Light()
: position_(glm::vec3(100.0f, 100.0f, 100.0f)), color_(glm::vec3(1.0f, 1.0f, 1.0f))
{ }

void Light::ImGui() {
    ImGui::DragFloat3("Position", &position_.x);
    ImGui::ColorEdit3("Color", &color_.x);
}
