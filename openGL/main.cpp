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
Camera camera;

float lstFrameTime = 0;

void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    const float cameraSpeed = 5.0f; // adjust accordingly
    const float cameraRotSpeed = 30.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.translateLocal(glm::vec3(0, 0, -cameraSpeed * (glfwGetTime() - lstFrameTime)));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.translateLocal(glm::vec3(0, 0, cameraSpeed * (glfwGetTime() - lstFrameTime)));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.translateLocal(glm::vec3(-cameraSpeed * (glfwGetTime() - lstFrameTime), 0, 0));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.translateLocal(glm::vec3(cameraSpeed * (glfwGetTime() - lstFrameTime), 0, 0));
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.translateLocal(glm::vec3(0, -cameraSpeed * (glfwGetTime() - lstFrameTime), 0));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.translateLocal(glm::vec3(0, cameraSpeed * (glfwGetTime() - lstFrameTime), 0));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.rotate(cameraRotSpeed * (glfwGetTime() - lstFrameTime), camera.right());
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.rotate(-cameraRotSpeed * (glfwGetTime() - lstFrameTime), camera.right());
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotate(cameraRotSpeed * (glfwGetTime() - lstFrameTime), glm::vec3(0, 1.0f, 0));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotate(-cameraRotSpeed * (glfwGetTime() - lstFrameTime), glm::vec3(0, 1.0f, 0));
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

    camera.setPosition(glm::vec3(0, 5, 10));
    camera.rotate(-25.0f, camera.right());
    Object3D *obj = new Object3D();
    res = obj->load("Resources/Rose.obj");
    // obj->rotate(90.0f, obj->right());
    // obj->rotate(90.0f, obj->right());
    obj->scale(glm::vec3(0.1f));
    if (!res) return -1;
    obj->setShader(&shader);

    obj->registerObject();

    GLint viewLoc = shader.getUniformLoc("view"),
        modelLoc = shader.getUniformLoc("model"),
        projLoc = shader.getUniformLoc("projection"),
        lightPosLoc = shader.getUniformLoc("lightPos"),
        lightColLoc = shader.getUniformLoc("lightColor"),
        viewPosLoc = shader.getUniformLoc("viewPos"),
        breathValLoc = shader.getUniformLoc("breathVal");

    lstFrameTime = glfwGetTime();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        
        processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setUniform(viewLoc, camera.getView());
        shader.setUniform(projLoc, camera.getProj());
        shader.setUniform(viewPosLoc, camera.getPosition());
        shader.setUniform(lightPosLoc, glm::vec3(0.0f, 0.0f, 40.0f));
        shader.setUniform(lightColLoc, glm::vec3(1.0f, 0.75f, 0.80f));
        
        shader.setUniform(breathValLoc, (float)glm::sin(glfwGetTime()));
        
        obj->rotate(5.0f *(float)(glfwGetTime() - lstFrameTime), obj->up());

        lstFrameTime = glfwGetTime();

        obj->display();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
