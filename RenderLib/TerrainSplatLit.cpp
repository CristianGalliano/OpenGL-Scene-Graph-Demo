#include "stdafx.h"
#include "TerrainSplatLit.h"
#include "TerrainGeometry.h"
#include "GeometryHelper.h"
#include <cmath>

void TerrainSplatLit::CreateGeometry(const TerrainGeometry& gHelper)
{
	// load the shader program.
	shader = new SOF::ShaderProgram("shaders/vshader_tex_terrain.glsl", "shaders/fshader_tex_terrain.glsl");
	// make the SOF::Geometry object.
	geom = new SOF::Geometry(gHelper.GetNumVertices());
	// add the position, normal, UV and SplatUV vector attributes.
	geom->AddAttribute(gHelper.GetPositionVector(), "vertexPos");
	geom->AddAttribute(gHelper.GetNormalVector(), "vertexNormal");
	geom->AddAttribute(gHelper.GetUVVector(), "vertexUV");
	geom->AddAttribute(gHelper.GetSplatUVVector(), "vertexSplatUV");
	// finish up.
	geom->Finalize(shader);
	// clean up.
}

void TerrainSplatLit::Draw(const Transforms& trans, const PointLight& light)
{
	// calculate the mvp matrix.
	glm::mat4 vp = trans.GetProj() * trans.GetView();
	// enable the shader, set uniforms.
	shader->Use();
	shader->SetUniformMat4("viewProj", vp);
	shader->SetUniformMat4("model", trans.GetModel());
	// get the camera position.
	glm::vec3 camPos = -glm::vec3(trans.GetView()[3]) * glm::mat3(trans.GetView());
	// set the lighting uniforms.
	shader->SetUniformVec3("lightK", glm::vec3(light.GetAmbientLevel(), light.GetDiffuseLevel(), light.GetSpecularLevel()));
	shader->SetUniformFloat("alpha", light.GetSpecularPower());
	shader->SetUniformMat4("lightPos", light.GetPosition());
	shader->SetUniformVec3("cameraPos", camPos);
	shader->SetUniformVec4("ambientColor", light.GetColor()[0]);
	shader->SetUniformMat4("lightColor", light.GetColor());
	glm::vec4 attConstant = glm::vec4(1, 1, 1, 1) / (light.GetAttenuationRadius() * light.GetAttenuationRadius());
	shader->SetUniformVec4("attenuation", attConstant);
	// hook up the textures and assign the splatmap texture.
	shader->AssignTextureSampler("tex0", textures[0]);
	shader->AssignTextureSampler("tex1", textures[1]);
	shader->AssignTextureSampler("tex2", textures[2]);
	shader->AssignTextureSampler("splat_map", SplatMap);
	// draw the triangles.
	geom->DrawPrimitives();
}
