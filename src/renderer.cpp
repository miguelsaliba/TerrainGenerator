#include "renderer.h"
#include "camera.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "terrain.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Renderer::Renderer()
    : window(Constants::WIDTH, Constants::HEIGHT, "Terrain Generator"),
    camera(),
    shader("../shaders/shader.vert", "../shaders/shader.frag"),
    terrain(100, 100)
{
    unsigned int VAO;
    terrain.generate_terrain();

    // simple way to set the callback of the pointer.
    glfwSetWindowUserPointer(window.getGLFWWindow(), &camera);
    auto func = [](GLFWwindow *w, double xpos, double ypos) {
        if (glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            static_cast<Camera*>(glfwGetWindowUserPointer(w))->mouse_callback(w, xpos, ypos);
        }
        ImGui_ImplGlfw_CursorPosCallback(w, xpos, ypos);
    };
    glfwSetCursorPosCallback(window.getGLFWWindow(), func);

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

    shader.use();
}

void Renderer::loop() {
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window.getGLFWWindow())) {
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("view", camera.lookAt());
        shader.setMat4("projection", camera.projection());
        glDrawElements(GL_TRIANGLES, terrain.size() * 2, GL_UNSIGNED_INT, 0);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // render your GUI
        ImGui::Begin("Demo window");
        ImGui::Button("Hello!");
        if (ImGui::Button("Generate Terrain")) {
            terrain.generate_terrain();
        }
        ImGui::End();

        // Render dear imgui into screen
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window.getGLFWWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.getGLFWWindow());
        key_callback(window.getGLFWWindow(), deltaTime);
    }
}

void Renderer::key_callback(GLFWwindow *w, float deltaTime) {
    if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        deltaTime *= 4; // Act as if more time passed so the movement speed is faster,
                        // there is definitely a better way of doing this but it's fine.
    }
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(FORWARD, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(BACKWARD, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(LEFT, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(RIGHT, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.move(UP, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.move(DOWN, deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}