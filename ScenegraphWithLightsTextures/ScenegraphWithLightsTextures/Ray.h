#ifndef RAY_H
#define RAY_H

#include <GL/gl.h>
#include <vector>
#include <stack>
#include "material.h"
#include "texture.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>

class Ray
{
public:


    Ray() {

    }

    ~Ray()
    {

    }
	void setStart(glm::vec4 startingPoint){
		start=startingPoint;
	}
	glm::vec4 getStart(){
		return start;
	}
	void setDirection(glm::vec4 theDirection){
		direction=theDirection;
	}
	glm::vec4 getDirection(){
		return direction;
	}




protected:
	// woop

private:
	glm::vec4 direction, start;
	
};

#endif // RAY_H
