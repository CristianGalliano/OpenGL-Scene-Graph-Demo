#pragma once
#include "Transforms.h"
#include "texture.h"
#include "GeometryHelper.h"
#include "PointLight.h"
#include "Drawable.h"
#include "TerrainGeometry.h"

class TerrainSplatLit : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	SOF::Texture* textures[3];
	SOF::Texture* SplatMap;
	void CreateGeometry(const TerrainGeometry& geomHelper);

public:
	TerrainSplatLit(const TerrainGeometry& geomHelper, const std::vector<std::string> textureFiles, const std::string& splatMapFile)
	{
		// use the passed in filepaths to assign the 3 textures in the array of textures.
		textures[0] = new SOF::Texture(textureFiles[0]);
		textures[1] = new SOF::Texture(textureFiles[1]);
		textures[2] = new SOF::Texture(textureFiles[2]);
		// used the passed in splat map file path to assign the splat map texture.
		SplatMap = new SOF::Texture(splatMapFile);
		CreateGeometry(geomHelper);
	}

	virtual void Draw(const Transforms& renderTransform, const PointLight& light) override;
};
