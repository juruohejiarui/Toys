#include "timectrl.hpp"
#include "particles.hpp"
#include <GLFW/glfw3.h>

ParticleSrc::ParticleSrc(size_t particleNum) {
	particles = new Particle[particleNum];
	for (int i = 0; i < particleNum; i++) freeQue.push(i);
	actList.clear();
	parObj.load("Resources/Particle.obj");
	parObj.registerObject();
	parObj.scale(glm::vec3(0.12f));
}

ParticleSrc::~ParticleSrc() {
	delete[] particles;
}

void ParticleSrc::display(Camera *camera) {
	const float timeGap = getCurTime() - lstFrameTime;
	for (int i = 0; i < actList.size(); i++) {
		int id = actList[i];
		parObj.setPosition(particles[id].pos);
		
		// parObj.lookAt(camera->getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
		shader->setUniform(this->shaderObjColLoc, glm::vec4(particles[id].col, particles[id].life));
		parObj.display();

		particles[id].pos += timeGap * particles[id].vel;
		particles[id].vel += timeGap * particles[id].acc;

		particles[id].life -= timeGap * particles[id].fade;
		
		// destroy this particles
		if (particles[id].life < 0.0f) {
			particles[id].active = false;
			freeQue.push(actList[i]);
			std::swap(actList[i], actList[actList.size() - 1]);
			actList.pop_back();
			i--;
		}
	}
}

int ParticleSrc::newParticle(float fade, const glm::vec3 &pos, const glm::vec3 &col, const glm::vec3 &vel, const glm::vec3 &acc) {
	if (freeQue.empty()) return 0;
	int id = freeQue.front(); freeQue.pop();
	particles[id].life = 1.0f;
	particles[id].fade = fade;
	particles[id].pos = pos;
	particles[id].col = col;
	particles[id].vel = vel;
	particles[id].acc = acc;
	actList.push_back(id);
	return 1;
}

void ParticleSrc::setShader(Shader *shader) {
    this->shader = shader;
	parObj.setShader(this->shader);
    this->shaderModelLoc = shader->getUniformLoc("model");
	this->shaderObjColLoc = shader->getUniformLoc("objColor");
}