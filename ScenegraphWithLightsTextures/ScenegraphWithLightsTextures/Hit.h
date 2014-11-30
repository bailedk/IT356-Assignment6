#ifndef HIT_H
#define HIT_H

#include <GL/gl.h>
#include <vector>
#include <stack>
#include "material.h"
#include "texture.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>

// there is a glm::normalize you fucking retard, have a nice day :)

class Hit
{
public:

	float time;
	//pt
	glm::vec4 intersect;
	//n_pt
	glm::vec4 normal;
	Material mat;
	Texture tex;
	// add a texturemap at some point?

	float s;
	float t;

    Hit() {
		t = -FLT_MAX*-1;
    }

    ~Hit()
    {

    }

	void setMat(Material material){
		mat=material;
	}
	void setIntersection(glm::vec4 intersectIn) {
		intersect = intersectIn;
	}
	void setNormal(glm::vec4 normalIn) {
		normal = normalIn;
	}
	glm::vec4 getIntersection() {
		return intersect;
	}
	Material getMat(){
		return mat;
	}
	void setT(float time){
		t = time;
	}
	float getT(){
		return t;
	}
protected:
	// woop
	
};

#endif // HIT_H
