#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include "spline.h"

class ExampleApp : public SOF::App
{
	// geometry
	TexturedLit* floorQuad;
	TexturedLit* cuboid;

	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;

	// scene graph
	Node* sceneGraphRoot;

	// spline points
	BSpline bSpline;
	float pointNum = 0;

public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};