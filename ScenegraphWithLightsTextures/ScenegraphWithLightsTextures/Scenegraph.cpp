#include "Scenegraph.h"
#include <stack>
#include "TransformNode.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <sstream> 
#include <iostream>
#include "Ray.h"
#include "Hit.h"
#include <algorithm>

Scenegraph::Scenegraph()
{
    root = NULL;
	trackballTransform = glm::mat4(1.0);

	camNum = 0;
	zoom = 0.0;
	turn = 0.0;
}

void Scenegraph::makeScenegraph(Node *r)
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
    this->root = r;

	cabin = static_cast<TransformNode*>(root->getNode("cabin"));
	chair0 = static_cast<TransformNode*>(root->getNode("chair0"));
	chair1 = static_cast<TransformNode*>(root->getNode("chair1"));
	chair2 = static_cast<TransformNode*>(root->getNode("chair2"));
	chair3 = static_cast<TransformNode*>(root->getNode("chair3"));



}



Scenegraph::~Scenegraph()
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
}

void Scenegraph::initShaderProgram(GLint shaderProgram)
{
	// hacky way to get this
	programCopy = shaderProgram;

	modelviewLocation = glGetUniformLocation(shaderProgram,"modelview");
	objectColorLocation = glGetUniformLocation(shaderProgram,"vColor");
	
   
	normalMatrixLocation = glGetUniformLocation(shaderProgram,"normalmatrix");
	numLightsLocation = glGetUniformLocation(shaderProgram,"numLights");

	
	mat_ambientLocation = glGetUniformLocation(shaderProgram,"material.ambient");
	mat_diffuseLocation = glGetUniformLocation(shaderProgram,"material.diffuse");
	mat_specularLocation = glGetUniformLocation(shaderProgram,"material.specular");
	mat_shininessLocation = glGetUniformLocation(shaderProgram,"material.shininess");

	
	texturematrixLocation = glGetUniformLocation(shaderProgram,"texturematrix");
	textureLocation = glGetUniformLocation(shaderProgram,"image");

}

void Scenegraph::draw(stack<glm::mat4>& modelView)
{
   
	
	if (root!=NULL)
	{
		
		getLights(modelView);

		for (int i=0;i<lights.size();i++)
		{
			stringstream name;

			name << "light[" << i << "].ambient";

			lightLocation[i].ambientLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			name << "light[" << i << "].diffuse";

			lightLocation[i].diffuseLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			name << "light[" << i << "].specular";

			lightLocation[i].specularLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			name << "light[" << i << "].position";

			lightLocation[i].positionLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());
		}


		glUniform1i(numLightsLocation,lights.size());

		for (int i=0;i<lights.size();i++){
			glUniform3fv(lightLocation[i].ambientLocation,1,glm::value_ptr(lights[i].getAmbient()));
			glUniform3fv(lightLocation[i].diffuseLocation,1,glm::value_ptr(lights[i].getDiffuse()));
			glUniform3fv(lightLocation[i].specularLocation,1,glm::value_ptr(lights[i].getSpecular()));
			glUniform4fv(lightLocation[i].positionLocation,1,glm::value_ptr(lights[i].getPosition()));
		}

		if (root!=NULL)
		{
			root->draw(modelView);
		}

		if (root!=NULL)
		{
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			root->updateBB();
			root->drawBB(modelView);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}
		
	}
}

void Scenegraph::animate(float time)
{
	/*
	time = time*8;
	if(cabin != NULL) {
               cabin->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(140,75,0)) * glm::rotate(glm::mat4(1.0),cos(time),glm::vec3(0,0,1)) * glm::translate(glm::mat4(1.0),glm::vec3(-100,-75,0)));
       }

       if(chair0 != NULL) {
               chair0->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
               chair1->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(cos(time*5)*10,0,0)));
               chair2->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
               chair3->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
       }

	TransformNode *	pole1 = static_cast<TransformNode*>(root->getNode("pole1"));
	if(pole1!=NULL){
		pole1->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*8.0f,glm::vec3(0,1,0)));
	}

	
	TransformNode *	pole2 = static_cast<TransformNode*>(root->getNode("pole2"));
	if(pole2!=NULL){
		pole2->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*-8.0f,glm::vec3(0,1,0)));
	}

	TransformNode *	pole3 = static_cast<TransformNode*>(root->getNode("pole3"));
	if(pole3!=NULL){
		pole3->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*8.0f,glm::vec3(0,1,0)));
	}

	TransformNode *	pole4 = static_cast<TransformNode*>(root->getNode("pole4"));
	if(pole4!=NULL){
		pole4->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*-8.0f,glm::vec3(0,1,0)));
	}
	
	TransformNode *	spinner = static_cast<TransformNode*>(root->getNode("spinner"));
	if(spinner!=NULL){
		spinner->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*8,glm::vec3(0,1,0)));
	}
	*/
}

void Scenegraph::getLights(stack<glm::mat4>& modelView){
	lights.clear();
	root->getLights(lights, modelView);
}

vector<vector<float>> Scenegraph::raytrace(int w, int h, stack<glm::mat4>& modelView){
	// don't think you really need to have this arr but whatevs
	vector<vector<float>> arr;

	getLights(modelView);

	// Create the image
	image.create(w,h);

	cout << "Starting raytrace" << endl;

	//source: http://stackoverflow.com/questions/4427662/whats-the-relationship-between-field-of-view-and-lens-length
	float focalLength = (h/2*1.0f)/(tan(fov/2));

	cout << "fov " << fov << endl;
	cout << "fieldofview " << focalLength << endl;
	int count = 0;
	for(int i = 0; i<h;i++){
		for(int j = 0; j<w; j++){
			
			if(i == h/2 && j == w/2) {
				cout << "midpoint" << endl;
			}

			Ray ray;
			ray.setStart(glm::vec4(0,0,0,1));
			ray.setDirection(glm::vec4((j-w/2),(h/2-i),-focalLength,0));

			if(raycast(ray, modelView, color)) {
				image.setPixel(j,i,color);
				count++;
				//cout << "white" << endl;
			}
			else {
				image.setPixel(j,i,color);
				//cout << "black" << endl;
			}
		}
	}

	cout << "count" << count << endl;

	image.saveToFile("raytrace.png");

	return arr;

}

//bool Scenegraph::raycast(Ray ray, stack<glm::mat4>& modelView, Material& mat, sf::Color& color){
bool Scenegraph::raycast(Ray ray, stack<glm::mat4>& modelView, sf::Color& color){
	bool isHit = false;
	Hit hit;
	isHit=root->intersect(ray,hit,modelView); 

	if(isHit) {
		color = shade(hit.intersect, lights, hit.normal, hit.getMat());
		hit;
		//cout << "hit set color "  << endl;
		// lots of other stuff for reflection, transparency, refract etc...
		// todo later
		//color = color.White;
	}
	else {
		color = color.Black;
	}

	return isHit;
}

sf::Color Scenegraph::shade(glm::vec4 pt, vector<Light>& lights, glm::vec4 normal, Material& mat) {


	glm::vec4 colorv = glm::vec4(0,0,0,1);

	glm::vec3 lightVec,viewVec,reflectVec;
	glm::vec3 normalView;
	glm::vec3 ambient,diffuse,specular;
	float nDotL,rDotV;

	for (int i=0;i<lights.size();i++)
	{
		if (lights[i].getPosition().w != 0)
			lightVec = glm::normalize(lights[i].getPosition().xyz()  - pt.xyz());
		else
			lightVec = glm::normalize(-lights[i].getPosition().xyz());

		glm::vec3 tNormal = normal.xyz();
		normalView = glm::normalize(tNormal);
		nDotL = glm::dot(normalView,lightVec);

		viewVec = -pt.xyz();
		viewVec = glm::normalize(viewVec);

		reflectVec = glm::reflect(-lightVec,normalView);
		reflectVec = glm::normalize(reflectVec);

		rDotV = glm::max(glm::dot(reflectVec,viewVec),0.0f);

		ambient = mat.getAmbient().xyz() * lights[i].getAmbient().xyz();

		diffuse = mat.getDiffuse().xyz() * lights[i].getDiffuse().xyz() * glm::max(nDotL,0.0f);

		if (nDotL>0) {
			specular = mat.getSpecular().xyz() * lights[i].getSpecular().xyz() * glm::pow(rDotV,mat.getShininess());
		}
		else {
			specular = glm::vec3(0,0,0);
		}
		//colorv = colorv + glm::vec4(ambient+diffuse+specular,1.0);
		//cout << "specular " << specular.x << " " << specular.y << " " << specular.z << " " << endl; 
		colorv = colorv + glm::vec4(ambient+diffuse+specular,1.0);
	}
		//fColor = fColor * texture2D(image,fTexCoord.st);
	
	//cout << "color: " << colorv.x << " " << colorv.y << " " << colorv.z << endl;

	int r = colorv.x* 255;
	int g = colorv.y* 255;
	int b = colorv.z* 255;
	int a = colorv.a* 255;
	
	if(r > 255)
		r = 255;
	if(g > 255)
		g = 255;
	if(b > 255) 
		b = 255;
	
	sf::Color colorr(r,g,b,a);
	return colorr;

}

void Scenegraph::setFOV(float fieldOfView){
		fov = fieldOfView;
}