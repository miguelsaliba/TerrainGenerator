#include "renderer.h"
#include "camera.h"
#include "imgui.h"
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
    shader("../shaders/mountain.vert", "../shaders/mountain.frag"),
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
    glfwSetKeyCallback(window.getGLFWWindow(), &Renderer::key_callback);

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

    shader.use();
}

void Renderer::loop() {
    float lastFrame = 0.0f;
    glm::vec3 lightPos(30.0f, 100.0f, 0.0f);
    while (!glfwWindowShouldClose(window.getGLFWWindow())) {
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        shader.use();
        shader.setMat4("view", camera.lookAt());
        shader.setMat4("projection", camera.projection());
        shader.setVec3("lightPos", lightPos);

        int display_w, display_h;
        glfwGetFramebufferSize(window.getGLFWWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow(); // Demo window for debugging
        ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_FirstUseEver);

        ImGui::Begin("Config", NULL, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("Generate")) {
                terrain.generate_terrain();
            }
            if (ImGui::MenuItem("Quit")) {
                glfwSetWindowShouldClose(window.getGLFWWindow(), true);
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("Click the \"Generate\" button or press \nEnter to apply changes!");

        if (ImGui::CollapsingHeader("Presets")) {
        }
        if (ImGui::CollapsingHeader("Noise")) {
            terrain.noise().ImGui();
        }
        if (ImGui::CollapsingHeader("Terrain")) {
            terrain.ImGui();
        }
        if (ImGui::CollapsingHeader("Camera")) {
            camera.ImGui();
        }
        ImGui::InputFloat3("LightPos", &lightPos[0]);
        ImGui::End(); // End Config

        glDrawElements(GL_TRIANGLES, terrain.size(), GL_UNSIGNED_INT, 0);

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.getGLFWWindow());
        key_handler(window.getGLFWWindow(), deltaTime);
    }
}

// Using this for buttons that get held down (movement)
void Renderer::key_handler(GLFWwindow *w, float deltaTime) {
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
    if (glfwGetKey(w, GLFW_KEY_ENTER) == GLFW_PRESS) {
        terrain.generate_terrain();
    }
}

// Using callback for things that need to be toggled so it doesn't get spammed
void Renderer::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    // wireframe
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        static bool wireframe = false;
        wireframe = !wireframe;
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
