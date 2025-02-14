#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <queue>
#include <set>
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"

struct Particle {
	bool active;
	float life;
	float fade;
	
	glm::vec3 pos, col, vel, acc;
};

struct ParticleSrc : BaseObject {
private:
	GLuint shaderModelLoc, shaderObjColLoc;
	Shader *shader;
	Particle *particles;
	std::queue<int> freeQue;
	std::vector<int> actList;
	Object3D parObj;
public:
	ParticleSrc(size_t particleNum);
	~ParticleSrc();
	void display(Camera *camera);

	int newParticle(float fade, const glm::vec3 &pos, const glm::vec3 &col, const glm::vec3 &vel, const glm::vec3 &acc);
	
	void setShader(Shader *shader);
};
