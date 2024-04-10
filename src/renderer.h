#ifndef RENDERER_H_
#define RENDERER_H_

#include "camera.h"
#include "imgui.h"
#include "light.h"
#include "shader.h"
#include "terrain.h"
#include "window.h"
#include "texture.h"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;
    void loop();
    void key_handler(GLFWwindow *w, float deltaTime);
    void set_background_color();
    void render_ImGui();
    void set_preset(std::string preset);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
    glm::vec3 background_color;
    ImGuiIO io;
    Light light;
    Window window;
    Camera camera;
    Shader shader;
    Terrain terrain;
    TextureHandler texture_handler;
};

#endif  // RENDERER_H_
