#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opengl/window.hpp>

int main() {
    GL_TEST::Window window(800 , 800 , "GL_TEST" , NULL);
    if (window.failed) {
        std::cerr << "\n[MAIN] Window Failed!\n";
        return 0;
    }

    if(!gladLoadGL()) {
        std::cerr << "\n[MAIN]GLAD didn't load!\n";
        return -1;
    }

    glViewport(0, 0, 800, 800);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window.window);

    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
    }

    return 0;
}
