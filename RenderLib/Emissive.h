#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"
#include "PointLight.h"
#include "Geometry.h"
#include "SphereGeom.h"
#include "Transforms.h"
#include "Drawable.h"

class Emissive : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	glm::vec4 color;
	void CreateGeometry( const GeometryHelper &gHelper ) 
	{
		shader = new SOF::ShaderProgram("shaders/v_emissive.glsl", "shaders/f_emissive.glsl");
		geom = new SOF::Geometry(gHelper.GetNumVertices());
		geom->AddAttribute(gHelper.GetPositionVector(), "vertexPos");
		geom->Finalize(shader);
	}
public:
	Emissive(const GeometryHelper& gHelper, const glm::vec4 &color ) : color(color) { CreateGeometry(gHelper); }
	void SetColor(const glm::vec4& newColor) { color = newColor; }
	virtual void Draw(const Transforms &trans, const PointLight &light ) override
	{
		glm::mat4 mvp;
		mvp = trans.GetProj() * trans.GetView() * trans.GetModel();
		shader->Use();
		shader->SetUniformMat4("mvp", mvp);
		shader->SetUniformVec4("color", color);
		geom->DrawPrimitives();
	}
};