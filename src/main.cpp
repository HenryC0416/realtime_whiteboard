#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "Canvas.hpp"

void error_callback(int error, const char* description) {
    std::cerr << "Error " << error << ": " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static Canvas* canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // Add color picker toggle with 'C' key
    else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        canvas->toggleColorPicker();
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        canvas->toggleBackgroundColorPicker();
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        canvas->setisEraser();
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    static Canvas* canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) { 
            canvas->handleMouseClick(mouseX, mouseY);
        } else if (action == GLFW_RELEASE) {
            canvas->endLine();
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    static Canvas* canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        canvas->handleMouseDrag(xpos, ypos);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set error callback
    glfwSetErrorCallback(error_callback);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Realtime Whiteboard", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Create canvas
    Canvas canvas(800, 600, window);
    glfwSetWindowUserPointer(window, &canvas);

    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Set up OpenGL viewport
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Draw canvas
        canvas.draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
} 