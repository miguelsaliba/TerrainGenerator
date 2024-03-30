#include "renderer.h"

#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


int main(int argc, char *argv[]) {
    Renderer renderer;
    renderer.loop();
}
