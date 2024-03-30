#ifndef RENDERER_H_
#define RENDERER_H_

#include "camera.h"
#include "shader.h"
#include "terrain.h"
#include "window.h"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;
    void loop();
    void key_callback(GLFWwindow *w, float deltaTime);
    void generate_terrain(unsigned int &VAO);

private:
    Window window;
    Camera camera;
    Shader shader;
    Terrain terrain;
};

#endif  // RENDERER_H_
