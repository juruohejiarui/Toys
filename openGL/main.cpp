#include <iostream>
#include <cstring>
#include <format>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "camera.hpp"
#include "object.hpp"

const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

GLFWwindow *window;

void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

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

    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram;
    int res = loadShaders(shaderProgram, "Shaders/vertex.vert", "Shaders/fragment.frag");
    if (!res) return -1;
    Shader shader(shaderProgram);

    Camera camera;
    camera.setPosition(glm::vec3(0, 0, 3));

    Object3D *obj = new Object3D();
    res = obj->load("Resources/Apple.obj");
    if (!res) return -1;
    obj->setShader(&shader);

    obj->registerObject();

    GLint viewLoc = shader.getUniformLoc("view"),
        modelLoc = shader.getUniformLoc("model"),
        projLoc = shader.getUniformLoc("projection"),
        lightPosLoc = shader.getUniformLoc("lightPos"),
        viewPosLoc = shader.getUniformLoc("viewPos"),
        lightColor = shader.getUniformLoc("lightColor");

    float lst = 0;
    while (!glfwWindowShouldClose(window))
    {
        processInput();

        glBindVertexArray(obj->getVAO());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setUniform(viewLoc, camera.getView());
        shader.setUniform(projLoc, camera.getProj());
        shader.setUniform(modelLoc, obj->getModelMat());
        shader.setUniform(viewPosLoc, camera.getPosition());
        shader.setUniform(lightPosLoc, glm::vec3(0.0f, 3.0f, 0.0f));
        shader.setUniform(lightColor, glm::vec3(1.0f, 0.75f, 0.80f));
        
        obj->rotateLocal(glm::vec3(0.1f, 0.0f, 0) * (float)(glfwGetTime() - lst));

        lst = glfwGetTime();

        obj->display();
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
