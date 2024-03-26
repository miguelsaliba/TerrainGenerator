#include "camera.h"
#include "constants.h"
#include "window.h"
#include "shader.h"
#include <vector>

#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void initImGui();
void mouse_callback(GLFWwindow *w, double xpos, double ypos);
void key_callback(GLFWwindow *w);
void generate_terrain(unsigned int &VAO, unsigned int &EBO);


Camera *camera;
Window *window;

int main(int argc, char *argv[]) {

    window = new Window(Constants::WIDTH, Constants::HEIGHT, "Terrain Generator");
    camera = new Camera(*window);

    Shader shader("shader.vert", "shader.frag");
    unsigned int VAO, EBO;
    generate_terrain(VAO, EBO);

    glfwSetCursorPosCallback(window->getGLFWWindow(), mouse_callback);

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)Constants::WIDTH / (float)Constants::HEIGHT, 0.1f, 900.0f);
    shader.use();

    while (!glfwWindowShouldClose(window->getGLFWWindow())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera->lookAt());
        glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window->getGLFWWindow());
        glfwPollEvents();
        key_callback(window->getGLFWWindow());
    }

    delete camera;
    delete window;
    return 0;
}

// TODO: move to terrain class
void generate_terrain(unsigned int &VAO, unsigned int &EBO) {
    // Create vertices
    std::vector<float> vertices{
        -0.5f, -0.5f, -0.9f,
         0.5f, -0.5f, -0.9f,
         0.0f,  0.5f, -0.9f
    };
    // TODO: change to terrain width and height
    //
    // for (int i = 0; i < Constants::HEIGHT; i++) {
    //     for (int j = 0; j < Constants::WIDTH; j++) {
    //         vertices.push_back(j);
    //         vertices.push_back(0.4);
    //         vertices.push_back(-i);
    //     }
    // }

    // Create indices
    std::vector<unsigned int> indices{0, 1, 2};
    // for (int i = 0; i < Constants::HEIGHT - 1; i++) {
    //     for (int j = 0; j < Constants::WIDTH - 1; j++) {
    //         indices.push_back(i       * Constants::WIDTH + j    );
    //         indices.push_back(i       * Constants::WIDTH + j + 1);
    //         indices.push_back((i + 1) * Constants::WIDTH + j + 1);
    //         indices.push_back(i       * Constants::WIDTH + j    );
    //         indices.push_back((i + 1) * Constants::WIDTH + j + 1);
    //         indices.push_back((i + 1) * Constants::WIDTH + j    );
    //     }
    // }

    // Create VAO, VBO, EBO
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO, VBO, EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void mouse_callback(GLFWwindow *w, double xpos, double ypos) {
    camera->mouse_callback(w, xpos, ypos);
}

void key_callback(GLFWwindow *w) {
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        camera->move_forward();
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        camera->move_backward();
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        camera->move_left();
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        camera->move_right();
    }
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, true);
    }
}
