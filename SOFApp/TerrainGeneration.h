#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "texture.h"
#include "GeometryHelper.h"
#include "PointLight.h"
#include "Drawable.h"
#include "RenderLib.h"

class TerrainGeneration : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	SOF::Texture* heightMap;

	int width, height;

	int gridScale, heightScale;

	void CreateGeometry(const GeometryHelper& geomHelper);

public:
	TerrainGeneration(const GeometryHelper& geomHelper)
	{
		heightMap = new SOF::Texture("textures/terrainMap.png");
		heightMap->EnableFiltering(false);

		width = heightMap->GetWidth();
		height = heightMap->GetHeight();
		int numTriangles = ((width - 1) * (height - 1)) * 2;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
			}
		}

		shader = new SOF::ShaderProgram("shaders/vshader_noise.glsl", "shaders/fshader_noise.glsl");
		// make the SOF::Geometry object
		geom = new SOF::Geometry(geomHelper.GetNumVertices());
		// add the position attribute
		geom->AddAttribute(geomHelper.GetPositionVector(), "vertexPos");
		geom->Finalize(shader);
	}
	virtual void Draw(const Transforms& renderTransform, const PointLight& light) override
	{
		// calculate the mvp matrix;
		glm::mat4 vp = renderTransform.GetProj() * renderTransform.GetView();
		// enable the shader, set uniforms
		shader->Use();
		shader->SetUniformMat4("viewProj", vp);
		shader->SetUniformMat4("model", renderTransform.GetModel());
		// hook up the texture
		shader->AssignTextureSampler("noiseSampler", heightMap);
		// set the noise scale
		shader->SetUniformFloat("iNoiseScale", 1.0f);
		// draw the triangles
		geom->DrawPrimitives();
	}
};