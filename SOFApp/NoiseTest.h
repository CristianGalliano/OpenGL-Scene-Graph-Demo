#pragma once
#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "texture.h"
#include "GeometryHelper.h"
#include "PointLight.h"
#include "Drawable.h"

class NoiseTest : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	SOF::Texture* noiseTex;

public:
	NoiseTest(const GeometryHelper& geomHelper )
	{
		noiseTex = new SOF::Texture("textures/grads.png");
		noiseTex->EnableFiltering(false);
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
		shader->AssignTextureSampler("noiseSampler", noiseTex);
		// set the noise scale
		shader->SetUniformFloat("iNoiseScale", 0.6f);
		// draw the triangles
		geom->DrawPrimitives();
	}
};