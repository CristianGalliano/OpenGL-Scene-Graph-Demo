#include "stdafx.h"
#include "exampleapp.h"

Node* Scene();

void ExampleApp::Init()
{
	// setup the camera.
	eye = glm::vec3(0.0, 0.0f, 0.0f);
	lookAt = glm::vec3(0.0f, 10.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!
	camera = new FlyCam(eye, lookAt);

	// create points for spline.
	std::vector<glm::vec3> splinePoints;
	splinePoints.push_back(glm::vec3(-128.0f, 40.0f, -128.0f));
	splinePoints.push_back(glm::vec3(-128.0f, 40.0f, 128.0f));
	splinePoints.push_back(glm::vec3(128.0f, 40.0f, 128.0f));
	splinePoints.push_back(glm::vec3(128.0f, 40.0f, -128.0f));

	// create spline.
	bSpline = BSpline(splinePoints, true);

	sceneGraphRoot = Scene();
	// render states.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ExampleApp::Draw()
{
	// camera.
	glm::mat4 view;
	glm::mat4 proj;

	// set the eye position to be the pointNum position on the spline.
	eye = bSpline.GetPoint(pointNum);
	//for use for player controlled camera.
	//eye = camera->GetPos();
	
	lookAt = camera->GetLookAt();
	up = camera->GetUp();
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 5000.0f);

	// Render the scene graph
	RenderVisitor rv(view, proj);
	rv.Traverse(sceneGraphRoot);
}

void ExampleApp::Update(float deltaT)
{
	// Upadate the scene graph
	UpdateVisitor uv(deltaT);
	uv.Traverse(sceneGraphRoot);

	// increment point num, if pointNum exceeds the number of points on the spline then we reset it back to 0 so it can loop around again.
	pointNum += 0.00125f;
	if (pointNum > 4)
	{
		pointNum -= 4;
	}

	// camera movement if camera is not moving along a spline, WASD move the camera and arrow keys rotate.
	/*if (SOF::Window::IsKeyDown(GLFW_KEY_W))
		camera->Move(100.0f*deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_S))
		camera->Move(-100.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_A))
		camera->Strafe(-100.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_D))
		camera->Strafe(100.0f * deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_LEFT))
		camera->Pan(90.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_RIGHT))
		camera->Pan(-90.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_UP))
		camera->LookUpDown(90.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_DOWN))
		camera->LookUpDown(-90.0f * deltaT);*/
}


