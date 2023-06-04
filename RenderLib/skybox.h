#pragma once
#include "ShaderProgram.h"
#include "Transforms.h"
#include "cubemap.h"
#include "Drawable.h"
#include "CubeGeom.h"

class SkyBox : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Cubemap* cubeMapTexture;
	CubeGeometry* geom;
	SOF::Geometry* geometry;
public:
	SkyBox(SOF::Cubemap* cubeMapTexture) : cubeMapTexture(cubeMapTexture)
	{
		geom = new CubeGeometry();
		geometry = new SOF::Geometry(geom->GetNumVertices());
		geometry->AddAttribute(geom->GetPositionVector(), "vertexPos");
		shader = new SOF::ShaderProgram("shaders/skybox_vs.glsl", "shaders/skybox_fs.glsl");
		geometry->Finalize(shader);
	}
	virtual void Draw(const Transforms& renderTransform, const PointLight& light) override
	{
		shader->Use();

		glm::vec3 camPos = -glm::vec3(renderTransform.GetView()[3]) * glm::mat3(renderTransform.GetView());
		glm::mat4 viewProj = renderTransform.GetProj() * renderTransform.GetView();
		shader->SetUniformVec3("cameraPos", camPos);
		shader->SetUniformMat4("viewProj", viewProj);
		shader->AssignCubemapSampler("cubeMap", cubeMapTexture);
		glDepthMask(GL_FALSE);
		geometry->DrawPrimitives();
		glDepthMask(GL_TRUE);
	}
};