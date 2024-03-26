#ifndef RENDERER_H_
#define RENDERER_H_

#include "camera.h"
#include "window.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init_camera();
    void init_window();
    void render();
    void update();

private:
    Camera *camera;
    Window *window;
};

#endif  // RENDERER_H_
