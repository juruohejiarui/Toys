#include <iostream>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "object.hpp"

const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

GLFWwindow *window;

void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLfloat traingle1[] = {
    0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 0.0f   // top left 
};
GLuint traingleIndices1[] = {
    0, 1, 3,
    1, 2, 3
};

int main(int argc, char** argv){
    glfwInit();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "StudyOpenGL", nullptr, nullptr);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Failed to initalize GLEW" << std::endl;
        return -1;
    }
    GLuint shaderProgram;
    loadShaders(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
