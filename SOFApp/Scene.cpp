#include "stdafx.h"
#include "RenderLib.h"
#include <random>
#include "Spline.h"

#pragma region Update Callbacks
// used to rotate objects on each update frame.
class RotatorCallback : public UpdateCallback
{
	TransformNode* trans;
	float rate;
	glm::vec3 axis;
	float t;
public:
	RotatorCallback(TransformNode* trans, float rate, glm::vec3 axis) : trans(trans), rate(rate), axis(axis), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		trans->SetTransform(glm::rotate(glm::mat4(), t * rate, axis));
	}
};

// moves objects along the spline and set their rotation on each update frame.
class splineMovementCallback : public UpdateCallback
{
	TransformNode* trans;
	TransformNode* rot;
	float t;
	float splinePoints;
	float speed;
	BSpline bs;
public:
	splineMovementCallback(TransformNode* trans, TransformNode* rot, float splinePoints, BSpline bs, float speed) : trans(trans), rot(rot), splinePoints(splinePoints), bs(bs), speed(speed), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += speed;

		// if t exceeds the numvber of splinepoints we know we have finished the loop so reset t back to 0.
		if (t > splinePoints)
		{
			t = 0;
		}
		glm::vec3 a = bs.GetPoint(t);
		glm::vec3 b = bs.GetPoint(t + speed);

		// returns the tangent of a and b, allowing the making the rotation face b from a.
		float angle = -std::atan2(b.z - a.z, b.x - a.x);

		// set the passed in translation and rotation.
		trans->SetTransform(glm::translate(glm::mat4(), bs.GetPoint(t)));
		rot->SetTransform(glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0)));
	}
};

// timer for the lights to turn off and on.
class turbineLightCallback : public UpdateCallback
{
	Emissive* light;
	float t;
	float numLights;
public:
	turbineLightCallback(Emissive* light, float numLights) : light(light), numLights(numLights), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt / numLights;

		// sets the colour of the light to invisible (0 alpha).
		if ((int)t % 2 == 0)
		{
			light->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
		}
		// sets the colour of the light to green.
		else if (t > 4)
		{
			t == 0;
			light->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}
};
#pragma endregion

Node* Scene()
{
	// create root node.
	TransformNode* rootNode = new TransformNode(glm::mat4());

	// create light lightnode.
	LightNode* light = new LightNode(glm::vec3(20, 500, 50), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.1, 10, 10000);

#pragma region Skybox Creation
	// pass in the the 6 images for the cubemap into the vector.
	std::vector<std::string> cubeMapFiles;
	cubeMapFiles.push_back("textures/cubemaps/Daylight Box_Top.bmp");
	cubeMapFiles.push_back("textures/cubemaps/Daylight Box_Bottom.bmp");
	cubeMapFiles.push_back("textures/cubemaps/Daylight Box_Right.bmp");
	cubeMapFiles.push_back("textures/cubemaps/Daylight Box_Left.bmp");
	cubeMapFiles.push_back("textures/cubemaps/Daylight Box_Front.bmp");
	cubeMapFiles.push_back("textures/cubemaps/Daylight Box_Back.bmp");

	// create the cubemap for the skybox using the cubemapfiles vector and use the cubemap to create the skybox.
	SOF::Cubemap* cubemap = new SOF::Cubemap(cubeMapFiles);
	SkyBox* skybox = new SkyBox(cubemap);
	GeometryNode* skyboxNode = new GeometryNode(skybox);
#pragma endregion

#pragma region Terrain Generation
	// create a terrain geometry object passing in our heightmap file location, height scale and width and depth scale.
	TerrainGeometry terrain("textures/terrainMap.png", .15f, 0.5f);

	// create vector of paths to pass through to terrain splat lit to recieve the textures. terrain splat lit passes those through the splatmap shader.
	std::vector<std::string> paths;
	paths.push_back("textures/rock_02_diffuse.jpg");
	paths.push_back("textures/Grass.jpg");
	paths.push_back("textures/gravel_01_diffuse.jpg");
	TerrainSplatLit* terrainRender = new TerrainSplatLit(terrain, paths, std::string("textures/$RGB_Channel_Splat_WM_002.jpg"));

	// create the translate, rotate and scale transform nodes for the terrain.
	TransformNode* terrainTranslate = new TransformNode(glm::translate(glm::mat4(), glm::vec3(-256.0f, 0.0f, -256.0f)));
	TransformNode* terrainRotate = new TransformNode(glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(1, 0, 0)));
	TransformNode* terrainScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1)));
#pragma endregion

#pragma region Hot Air Balloon
	// create a vector of vec3 to create a spline.
	std::vector<glm::vec3> splinePoints;
	splinePoints.push_back(glm::vec3(250.0f, 100.0f, 250.0f));
	splinePoints.push_back(glm::vec3(250.0f, 50.0f, 0.0f));
	splinePoints.push_back(glm::vec3(250.0f, 100.0f, -250.0f));
	splinePoints.push_back(glm::vec3(0.0f, 50.0f, -250.0f));
	splinePoints.push_back(glm::vec3(-250.0f, 100.0f, -250.0f));
	splinePoints.push_back(glm::vec3(-250.0f, 50.0f, 0.0f));
	splinePoints.push_back(glm::vec3(-250.0f, 100.0f, 250.0f));
	splinePoints.push_back(glm::vec3(0.0f, 50.0f, 250.0f));

	// create a spline along the spline points, setting looping to true.
	BSpline BalloonPath = BSpline(splinePoints, true);

	// load in the balloon object and apply its textures.
	ObjLoader balloon("11088_balloon_v3.obj", "models/HotAirBalloon/");
	TexturedLit* balloonRender = new TexturedLit(balloon.GetGeometry(), "textures/balloonTexture.jpg");

	// create the balloons translate, rotate and scale transform nodes.
	TransformNode* balloonTranslate = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 21.75f, 0.0f)));
	TransformNode* balloonRotate = new TransformNode(glm::mat4());
	TransformNode* balloonScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(.125f, .125f, .125f)));

	// set the balloons update callback, this allows it to move along the spline and rotate towards the next point on the spline.
	balloonTranslate->SetUpdateCallback(new splineMovementCallback(balloonTranslate, balloonRotate, splinePoints.size(), BalloonPath, 0.001f));
#pragma endregion

#pragma region Wind Turbine
	// determines how many tansform nodes will be added to the groupnode for geometry instancing.
	int numTurbines = 50;

	// load in the turbine base model and its textures.
	ObjLoader turbineBase("WindTurbine_Base.obj", "models/Wind_Turbine/");
	TexturedLit* turbineBaseRender = new TexturedLit(turbineBase.GetGeometry(), "models/Wind_Turbine/Turbine_Texture.png");

	// set the turbine bases rotate and scale transform nodes, the translate node for this object will be added in the groupnode.
	TransformNode* turbineBaseRotate = new TransformNode(glm::mat4());
	TransformNode* turbineBaseScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(.5f, 0.5f, 0.5f)));

	// load in turbine fan model and its textures.
	ObjLoader turbineFan("WindTurbine_Turbine.obj", "models/Wind_Turbine/");
	TexturedLit* turbineFanRender = new TexturedLit(turbineFan.GetGeometry(), "models/Wind_Turbine/Turbine_Texture.png");

	// set the turbine fans translate, rotate and scale transform nodes.
	TransformNode* turbineFanTranslate = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 21.75f, 0.0f)));
	TransformNode* turbineFanRotate = new TransformNode(glm::mat4());
	TransformNode* turbineFanScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f)));

	// set the turbine fans update callback, this rotates the turbine each update frame making it spin.
	turbineFanRotate->SetUpdateCallback(new RotatorCallback(turbineFanRotate, 7.5f / numTurbines, glm::vec3(0.0f, 0.0f, 1.0f)));

	// create an emmissive sphere.
	SphereGeometry turbineLight(100);
	Emissive* turbinelightRender = new Emissive(turbineLight, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	// set the turbine lights translate, rotate and scale transform nodes.
	TransformNode* turbineLightTranslate = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 22.25f, 0.0f)));
	TransformNode* turbineLightRotate = new TransformNode(glm::mat4());
	TransformNode* turbineLightScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.25f, 0.25f, 0.25f)));

	// set the turbine lights update callback, this makes the light flicker on and off after a set interval.
	turbineLightTranslate->SetUpdateCallback(new turbineLightCallback(turbinelightRender, numTurbines));
#pragma endregion

#pragma region SceneGraph Hierarchy
	// add light as a child of the rootnode.
	rootNode->AddChild(light);

	// add the skybox node as a child of light.
	light->AddChild(skyboxNode);

	// subgraph for terrain transforms
	terrainTranslate->AddChild(terrainRotate);
	terrainRotate->AddChild(terrainScale);
	terrainScale->AddChild(new GeometryNode(terrainRender));

	// add terrainTranslate as a child of light.
	light->AddChild(terrainTranslate);

	// subgraph for balloon.
	balloonTranslate->AddChild(balloonRotate);
	balloonRotate->AddChild(balloonScale);
	balloonScale->AddChild(new GeometryNode(balloonRender));

	// add balloonTranslate as a child of light.
	light->AddChild(balloonTranslate);

	// create a groupnode to hold all turbines.
	GroupNode* turbineGroup = new GroupNode();
	glm::mat4 turbineTranslate;

	// will create as many turbines as we define in numTurbines.
	for (int i = 0; i < numTurbines; i++)
	{
		// set turbine translate to point on the terrain, we do this by passing a randomised vector 3 into return closest node on the terrain, which will clip it to the nearest point.
		turbineTranslate = glm::translate(glm::mat4(), terrain.returnClosestNode(glm::vec3((rand() % 256) + 128, 0, (rand() % 256) + 128)) - glm::vec3(0, 1, 0));

		// add turbineTranslate as a child of the groupnode, then add turbine rotate as a child of the i'th child of group node (the turbine translate node we just added).
		turbineGroup->AddChild(new TransformNode(turbineTranslate));
		turbineGroup->GetChild(i)->AddChild(turbineBaseRotate);
	}

	// subgraph for turbine.
	turbineBaseRotate->AddChild(turbineBaseScale);
	turbineBaseScale->AddChild(new GeometryNode(turbineBaseRender));

	turbineBaseScale->AddChild(turbineFanTranslate);
	turbineFanTranslate->AddChild(turbineFanRotate);
	turbineFanRotate->AddChild(turbineFanScale);
	turbineFanScale->AddChild(new GeometryNode(turbineFanRender));

	turbineBaseScale->AddChild(turbineLightTranslate);
	turbineLightTranslate->AddChild(turbineLightRotate);
	turbineLightRotate->AddChild(turbineLightScale);
	turbineLightScale->AddChild(new GeometryNode(turbinelightRender));

	// add the turbine groupnode as a child of terrain.
	terrainScale->AddChild(turbineGroup);
#pragma endregion

	return light;
}
