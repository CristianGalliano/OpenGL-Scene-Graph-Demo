#pragma once
#include "Drawable.h"
#include "ShaderProgram.h"
#include "Geometry.h"

class PolyLine : public Drawable
{
	SOF::ShaderProgram* Shader;
	SOF::Geometry* geometry;
	glm::vec4 color;
public:
	PolyLine(const std::vector<glm::vec3>& points, const glm::vec4& color) : color(color)
	{
		Shader = new SOF::ShaderProgram("shaders/v_emissive.glsl", "shaders/f_emissive.glsl");
		geometry = new SOF::Geometry(points.size());
		geometry->AddAttribute(points, "vertexPos");
		geometry->Finalize(Shader);
	}
	virtual void Draw(const Transforms& t, const PointLight& l)
	{
		Shader->Use();
		Shader->SetUniformMat4("mvp", t.GetProj() * t.GetView() * t.GetModel());
		Shader->SetUniformVec4("color", color);
		geometry->DrawLines();
	}
};