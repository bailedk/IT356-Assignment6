#ifndef GROUPNODE_H
#define GROUPNODE_H

#include <GL/gl.h>
#include <vector>
#include <stack>
using namespace std;
#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Light.h"


class GroupNode: public Node
{
protected:
    vector<Node *> children;

public:
    GroupNode(Scenegraph *graph,string name="")
        :Node(graph,name)
    {

    }

    ~GroupNode()
    {
        for (unsigned int i=0;i<children.size();i++)
            delete children[i];
    }

	virtual Node *clone()
	{
		vector<Node *> newc;

		for (int i=0;i<children.size();i++)
		{
			newc.push_back(children[i]->clone());
		}

		GroupNode * newgroup = new GroupNode(scenegraph,name);

		for (int i=0;i<children.size();i++)
		{
			newgroup->addChild(newc[i]);
		}		
		return newgroup;

	}

	virtual Node* getNode(string name)
	{
		if (Node::getNode(name)!=NULL)
		{
			return Node::getNode(name);
		}

		int i=0;
		Node *answer = NULL;

		while ((i<children.size()) && (answer == NULL))
		{
			answer = children[i]->getNode(name);
			i++;
		}
		return answer;
	}

    virtual void draw(stack<glm::mat4> &modelView)
    {
        for (int i=0;i<children.size();i++)
        {
            children[i]->draw(modelView);
        }
    }

	virtual void drawBB(stack<glm::mat4>& modelView)
	{
		if (bbDraw)
		{
			glm::mat4 bbTransform;

			bbTransform = glm::translate(glm::mat4(1.0),0.5f*(minBounds+maxBounds)) * glm::scale(glm::mat4(1.0),maxBounds-minBounds);
			glm::vec4 color = glm::vec4(1,1,1,1);
			//set the color for all vertices to be drawn for this object
			glUniform3fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			glUniformMatrix4fv(scenegraph->modelviewLocation,1,GL_FALSE,glm::value_ptr(modelView.top() * bbTransform));
			scenegraph->getInstance("box")->draw();        		
		}
		for (int i=0;i<children.size();i++)
        {
            children[i]->drawBB(modelView);
        }
	}

	virtual void updateBB()
	{
		int i,j;

		if (children.size()==0)
			return;

		for (i=0;i<children.size();i++)
		{
			children[i]->updateBB();
		}

		minBounds = children[0]->getMinBounds();
		maxBounds = children[0]->getMaxBounds();

		for (i=1;i<children.size();i++)
		{
			for (j=0;j<3;j++)
			{
				if (children[i]->getMinBounds()[j]<minBounds[j])
					minBounds[j] = children[i]->getMinBounds()[j];

				if (children[i]->getMaxBounds()[j]>maxBounds[j])
					maxBounds[j] = children[i]->getMaxBounds()[j];
			}
		}
	}
	virtual void getLights(vector<Light>& l, stack<glm::mat4>& modelView){
		
		for(int i =0; i<children.size(); i++){
			children[i]->getLights(l, modelView);
		}
		
		for(int i =0; i<lights.size();i++){
			Light light = lights[i];
			light.setPosition(glm::vec3(modelView.top()*lights[i].getPosition()));
			l.push_back(light);
		}
		
	}

    void addChild(Node *child)
    {
        children.push_back(child);
		child->setParent(this);
    }

    const vector<Node *> getChildren() const
    {
        return children;
    }

	virtual glm::mat4 getCameraTransform(){
		return glm::mat4(1.0);
	}

	virtual glm::mat4 getTransform(){
		return glm::mat4(1.0);
	}
	virtual bool intersect(Ray ray, Hit& hit,stack<glm::mat4>& modelView){
		bool hasHit = false;
		for(int i =0; i<children.size(); i++){
			if(children[i]->intersect(ray, hit, modelView)){
				hasHit=true;
			}
		}
		return hasHit;
	}

	
};

#endif // GROUP_H
