#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "texture.h"
#include "GeometryHelper.h"
#include "PointLight.h"
#include "Drawable.h"
#include "cubemap.h"

class TexturedLitEnv : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	SOF::Texture* texture;
	SOF::Cubemap* envMap;
	void CreateGeometry(const GeometryHelper& geomHelper);
public:
	TexturedLitEnv(const GeometryHelper& geomHelper, const std::string& texturFile, SOF::Cubemap* envMap) : envMap(envMap)
	{
		texture = new SOF::Texture(texturFile);
		CreateGeometry(geomHelper);
	}
	virtual void Draw(const Transforms& renderTransform, const PointLight& light) override;
};