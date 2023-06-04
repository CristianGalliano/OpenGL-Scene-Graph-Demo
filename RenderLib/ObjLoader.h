#pragma once
#include "GeometryHelper.h"
#include "tiny_obj_loader.h"
#include "obj_util.h"
#include <map>

class ObjGeometry : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
public:
	ObjGeometry() {}
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }// same as positions 
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

	void AddPosition(const glm::vec3& pos) { positions.push_back(pos); }
	void AddNormal(const glm::vec3& normal) { normals.push_back(normal); }
	void AddTangent(const glm::vec3& tangent) { tangents.push_back(tangent); }
	void AddUV(const glm::vec2& uv) { uvs.push_back(uv); }
};

class ObjLoader
{
	ObjGeometry geom;
	std::string textureFile;
public:
	ObjLoader(const std::string& fileName, const std::string &directory )
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		std::string warn;
		std::string filePath = directory + fileName;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &warn, filePath.c_str(), directory.c_str(), true))
		{
			std::cout << "error loading " << filePath << std::endl;
			std::cout << "error is " << err << std::endl;
		}
		glm::vec3 pos, norm;
		glm::vec2 uv;
		int numVerts = shapes[0].mesh.indices.size();
			// check if there are normals, compute them if not
		std::map<int, ObjUtil::vec3> smoothNormals;
		if (shapes[0].mesh.indices[0].normal_index == -1)
			ObjUtil::computeSmoothingNormals(attrib, shapes[0], smoothNormals);

		for (int i = 0; i < numVerts; i++)
		{
			int ivert, inorm, itex;
			ivert = shapes[0].mesh.indices[i].vertex_index;
			inorm = shapes[0].mesh.indices[i].normal_index;
			itex = shapes[0].mesh.indices[i].texcoord_index;

			pos.x = attrib.vertices[ivert * 3 + 0];
			pos.y = attrib.vertices[ivert * 3 + 1];
			pos.z = attrib.vertices[ivert * 3 + 2];
			if (inorm != -1)
			{
				norm.x = attrib.normals[inorm * 3 + 0];
				norm.y = attrib.normals[inorm * 3 + 1];
				norm.z = attrib.normals[inorm * 3 + 2];
			}
			else
			{
				norm.x = smoothNormals[ivert].v[0];
				norm.y = smoothNormals[ivert].v[1];
				norm.z = smoothNormals[ivert].v[2];
			}
			uv.x = attrib.texcoords[itex * 2 + 0];
			uv.y = 1.0f-attrib.texcoords[itex * 2 + 1];

			geom.AddPosition(pos);
			geom.AddNormal(norm);
			geom.AddUV(uv);
		}
		textureFile = directory + materials[0].diffuse_texname;
	}
	const GeometryHelper& GetGeometry() const{ return geom; }
	const std::string& GetTextureFileName() const { return textureFile; }
};