#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include <stack>
#include <iostream>
using namespace std;
#include "Light.h"
#include "Ray.h"
#include "Hit.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class Scenegraph;

class Node
{
protected:
    string name;
    Scenegraph *scenegraph;
	bool bbDraw;
	glm::vec3 minBounds,maxBounds;
	vector<Light> lights;
	bool camDraw;
	glm::mat4 mv;
	
public:
    Node(Scenegraph *graph,string name="")
    {
		this->parent = NULL;
        scenegraph = graph;
        setName(name);
		bbDraw = false;
		camDraw = false;
		mv = glm::mat4(1.0f);
    }

    virtual ~Node()
    {
	}

	virtual Node *getNode(string name)
	{
		if (this->name == name)
			return this;

		return NULL;
	}

	virtual glm::mat4 getTransform()=0;

    virtual void draw(stack<glm::mat4>& modelView)=0;
	virtual void drawBB(stack<glm::mat4>& modelView)=0;
	virtual void updateBB()=0;
	virtual Node *clone()=0;
	virtual void getLights(vector<Light>& l,stack<glm::mat4>& modelView)=0;
	virtual glm::mat4 getCameraTransform()=0;
	virtual bool intersect(Ray ray, Hit& hit,stack<glm::mat4>& modelView)=0;
	void setParent(Node *parent)
	{
		this->parent = parent;
	}
	Node * getParent()
	{
		return this->parent;
	}

    void setName(const string& name)
    {
        this->name = name;
    }

	void setBBDraw(bool d)
	{
		bbDraw = d;
	}

	void setCamDraw(bool c) {
		camDraw = c;
		//cout << "CAM DRAW!! " << endl;
	}

	glm::vec3 getMinBounds()
	{
		return minBounds;
	}

	glm::vec3 getMaxBounds()
	{
		return maxBounds;
	}

	void addLight(const Light& l)
	{
		//cout << "Light added in node " << name << endl;
		lights.push_back(l);
	}
	

protected:
	Node *parent;
};

#endif // NODE_H
