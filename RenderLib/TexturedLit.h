#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "texture.h"
#include "GeometryHelper.h"
#include "PointLight.h"
#include "Drawable.h"

class TexturedLit : public Drawable
{
	SOF::ShaderProgram *shader;
	SOF::Geometry *geom;
	SOF::Texture* texture;
	SOF::Texture* normalMap;
	void CreateGeometry(const GeometryHelper &geomHelper);

public:
	TexturedLit(const GeometryHelper& geomHelper, const std::string& textureFile, const std::string& normalMapFile)
	{
		texture = new SOF::Texture(textureFile);
		normalMap = new SOF::Texture(normalMapFile);
		CreateGeometry(geomHelper);
	}
	TexturedLit(const GeometryHelper& geomHelper, const std::string& textureFile )
	{
		texture = new SOF::Texture(textureFile);
		normalMap = nullptr;
		CreateGeometry(geomHelper);
	}

	virtual void Draw( const Transforms &renderTransform, const PointLight &light ) override;
};