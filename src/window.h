#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    void startLoop();
    GLFWwindow *getGLFWWindow() {return window;};
    ~Window();

private:
    int width;
    int height;
    GLFWwindow* window;
};

#endif // !WINDOW_H
