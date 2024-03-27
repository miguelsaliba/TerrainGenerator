#include "camera.h"
#include "constants.h"
#include "window.h"
#include "shader.h"
#include "perlin.h"
#include <vector>

#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


void initImGui();
void mouse_callback(GLFWwindow *w, double xpos, double ypos);
void key_callback(GLFWwindow *w, float deltaTime);
void generate_terrain(unsigned int &VAO);


Camera *camera;
Window *window;

int main(int argc, char *argv[]) {

    window = new Window(Constants::WIDTH, Constants::HEIGHT, "Terrain Generator");
    camera = new Camera(*window);

    Shader shader("shader.vert", "shader.frag");
    unsigned int VAO;
    generate_terrain(VAO);

    glfwSetCursorPosCallback(window->getGLFWWindow(), mouse_callback);

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)Constants::WIDTH / (float)Constants::HEIGHT, 0.1f, 100.0f);
    shader.use();

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window->getGLFWWindow())) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", camera->lookAt());
        shader.setMat4("projection", projection);
        glDrawElements(GL_TRIANGLES, 100*100, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window->getGLFWWindow());
        glfwPollEvents();
        key_callback(window->getGLFWWindow(), deltaTime);
    }

    delete camera;
    delete window;
    return 0;
}

// TODO: move to terrain class
void generate_terrain(unsigned int &VAO) {
    // Create vertices
    Perlin perlin(14);
    std::vector<float> vertices;
    // TODO: change to terrain width and height
    int terWidth = 100;
    int terHeight = 100;
    for (int i = 0; i < terHeight; i++) {
        for (int j = 0; j < terWidth; j++) {
            float x = (float)j / terWidth * 256;
            float z = (float)i / terHeight * 256;
            vertices.push_back(x);
            vertices.push_back(perlin.octaveNoise(x, z, 8, 0.5f));
            vertices.push_back(z);
            // std::cout << j<< " " << perlin.octaveNoise(j, i, 4, 0.5f) << " " << i<< std::endl;
        }
    }

    // Create indices
    std::vector<unsigned int> indices;
    for (int i = 0; i < terHeight - 1; i++) {
        for (int j = 0; j < terWidth - 1; j++) {
            indices.push_back(i       * terWidth + j    );
            indices.push_back(i       * terWidth + j + 1);
            indices.push_back((i + 1) * terWidth + j + 1);
            indices.push_back(i       * terWidth + j    );
            indices.push_back((i + 1) * terWidth + j + 1);
            indices.push_back((i + 1) * terWidth + j    );
        }
    }

    // Create VAO, VBO, EBO
    unsigned int VBO, EBO;
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

void key_callback(GLFWwindow *w, float deltaTime) {
    if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        deltaTime *= 4;
    }
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        camera->move(FORWARD, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        camera->move(BACKWARD, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        camera->move(LEFT, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        camera->move(RIGHT, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->move(UP, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera->move(DOWN, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, true);
    }
}
