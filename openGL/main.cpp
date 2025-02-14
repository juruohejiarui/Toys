#include <algorithm>
#include <iostream>
#include <cstring>
#include <format>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "timectrl.hpp"
#include "particles.hpp"

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

std::vector<glm::vec3> sorted_vertices;
float mxLen = 0.0f;
void get_sorted_vertices(Object3D *obj) {
	auto _cmp = [&](const glm::vec3 &a, const glm::vec3 &b) { return a.y < b.y; };
	for (int i = 0; i < obj->getNumVertices(); i++)
		sorted_vertices.push_back(obj->getVertex(i).pos),
		mxLen = std::max(mxLen, sorted_vertices[sorted_vertices.size() - 1].y);
	std::sort(sorted_vertices.begin(), sorted_vertices.end(), _cmp);
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

	ParticleSrc *parSrc = new ParticleSrc(1ul << 20);

	camera.setPosition(glm::vec3(0, 4, 8));
	camera.rotate(-25.0f, camera.right());
	Object3D *obj = new Object3D();
	res = obj->load("Resources/Rose.obj");

	obj->scale(glm::vec3(0.1f));
	parSrc->scale(glm::vec3(0.1f));
	obj->setPosition(glm::vec3(0, -3.0f, 0));
	// parSrc.setPosition(glm::vec3(0, -3.0f, 0));
	
	if (!res) return -1;
	obj->setShader(&shader);
	parSrc->setShader(&shader);

	obj->registerObject();

	get_sorted_vertices(obj);

	GLint viewLoc = shader.getUniformLoc("view"),
		modelLoc = shader.getUniformLoc("model"),
		projLoc = shader.getUniformLoc("projection"),
		lightPosLoc = shader.getUniformLoc("lightPos"),
		lightColLoc = shader.getUniformLoc("lightColor"),
		viewPosLoc = shader.getUniformLoc("viewPos"),
		discLyrLoc = shader.getUniformLoc("discLyr"),
		objColLoc = shader.getUniformLoc("objColor");
	lstFrameTime = glfwGetTime();

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int pos = 200;

	const auto pointCol = glm::vec4(0.5f, 0, 0, 1.0f), lineCol = glm::vec4(0.7f, 0, 0, 1.0f), fillCol = glm::vec4(1.0f, 0, 0, 1.0f);
	const auto pointSize = 4.5f, lineSize = 3.0f;

	auto drawPoint = [&]() {
		shader.setUniform(objColLoc, pointCol);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(pointSize);
		obj->display();
	};
	auto drawLine = [&] {
		shader.setUniform(objColLoc, lineCol);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(lineSize);
		obj->display();
	};
	auto drawFill = [&] {
		shader.setUniform(objColLoc, fillCol);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		obj->display();
	};

	float state0 = 15.0f, state1 = state0 + 20.0f, state2 = state1 + 3.0f, state3 = state2 + 5.0f;

	while (!glfwWindowShouldClose(window))
	{	
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.setUniform(viewLoc, camera.getView());
		shader.setUniform(projLoc, camera.getProj());
		shader.setUniform(viewPosLoc, camera.getPosition());
		shader.setUniform(lightPosLoc, glm::vec3(0.0f, 0.0f, 40.0f));
		shader.setUniform(lightColLoc, glm::vec3(1.0f, 0.75f, 0.80f));
		
		
		obj->rotate(20.0f * (getCurTime() - lstFrameTime), obj->up());
		parSrc->rotate(20.0f * (getCurTime() - lstFrameTime), obj->up());
		if (getCurTime() > state0 && getCurTime() <= 10 + state0)
			camera.rotateAround(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 5.0f * (getCurTime() - lstFrameTime)),
			camera.translateGlobal(glm::vec3(0, 3 * (getCurTime() - lstFrameTime), 6 * (getCurTime() - lstFrameTime)));
		if (getCurTime() <= state0) {

		} else if (getCurTime() <= state1) {
			float disLyr = (mxLen + 1.0f) * ((getCurTime() - state0) / (state1 - state0));
			shader.setUniform(discLyrLoc, disLyr);
			drawPoint();

			for (; pos < sorted_vertices.size(); pos += rand() % 5) {
				if (sorted_vertices[pos].y - 5.0f > disLyr) break;
				const auto parPos = glm::vec3(obj->getModelMat() * glm::vec4(sorted_vertices[pos], 1.0f));
				printf("(%.5f, %.5f, %.5f)\r", parPos.x, parPos.y, parPos.z);
				glm::vec3 vel = glm::vec3(rand() % 50 / 50.0f, rand() % 50 / 50.0f, rand() % 50 / 50.0f);
				if (!parSrc->newParticle(10.0f, parPos, glm::vec3(1.0f, 0.2f, 0.2f), 10.0f * vel, -2.0f * vel)) break;
			}

			disLyr = (mxLen + 0.2f) * ((getCurTime() - state0) / (state1 - state0));
			shader.setUniform(discLyrLoc, disLyr);
			drawLine();

		} else if (getCurTime() <= state2) {
			shader.setUniform(discLyrLoc, mxLen + 0.5f);
			drawPoint();

			drawLine();

			float disLyr = (mxLen + 0.5) * ((getCurTime() - state1) / (state2 - state1));
			shader.setUniform(discLyrLoc, disLyr);
			drawFill();
		} else if (getCurTime() <= state3) {
			shader.setUniform(discLyrLoc, mxLen + 0.5f);
			drawFill();

			float disLyr = (mxLen + 1.0f) * (1.0f - (getCurTime() - state2) / (state3 - state2));
			shader.setUniform(discLyrLoc, disLyr);
			drawLine();
			drawPoint();

		} else {
			shader.setUniform(discLyrLoc, mxLen + 0.5f);
			drawFill();
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		parSrc->display(&camera);
		
		printf("%.3lf\r", getCurTime());

		processInput();

		glfwPollEvents();
		glfwSwapBuffers(window);

		updateTime();
	}
	delete parSrc;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
