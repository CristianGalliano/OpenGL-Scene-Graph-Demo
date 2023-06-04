#pragma once
#include "GeometryHelper.h"

class TerrainGeometry : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec2> splatUVs;
	std::vector<glm::vec3> points;
	float ScaleWidthDepth;
	int width, depth;


	void CreateGeometry(const char* directory, float scaleHeight, float scaleWidthDepth)
	{
		// set the global ScaleWidthDepth to equal the passed in one.
		ScaleWidthDepth = scaleWidthDepth;

		// load in the heightmap as a texture and get its width and depth values.
		SOF::Texture* heightMap = new SOF::Texture(directory);
		width = (heightMap->GetWidth());
		depth = (heightMap->GetHeight());
		
		// create vectors for the face normals and the traingles.
		std::vector<glm::vec3> faceNormals;
		std::vector<int> triangles;

		// load in the greyscale values from the image for heightmapping.
		unsigned char* image = SOIL_load_image(directory, &width, &depth, 0, SOIL_LOAD_L);

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < depth; j++)
			{
				// set k to be the value passed back from the image array of values at the current position in the loop.
				float k = static_cast<float>(image[i * width + j]);

				// push back the position of the point to the points vector, we scale the width, depth and height based off of the scale values passed in when creating the terrain object.
				points.push_back(glm::vec3(i * scaleWidthDepth, k * scaleHeight, j * scaleWidthDepth));

				// if this point is on the edge of the terrain return a faceNormal of (0,1,0).
				if (i == 0 || j == 0 || i == width - 1 || j == depth - 1)
				{
					faceNormals.push_back(glm::vec3(0, 1, 0));
				}
				else
				{
					glm::vec3 directions [4];
					// point above.
					float k = static_cast<float>(image[i * width + (j + 1)]);
					directions[0] = glm::vec3(i * scaleWidthDepth, k * scaleHeight, (j + 1) * scaleWidthDepth);

					// point below.
					k = static_cast<float>(image[i * width + (j - 1)]);
					directions[1] = glm::vec3(i * scaleWidthDepth, k * scaleHeight, (j - 1) * scaleWidthDepth);

					// point left.
					k = static_cast<float>(image[(i - 1) * width + j]);
					directions[2] = glm::vec3((i - 1) * scaleWidthDepth, k * scaleHeight, j * scaleWidthDepth);

					// point right.
					k = static_cast<float>(image[(i + 1) * width + j]);
					directions[3] = glm::vec3((i + 1) * scaleWidthDepth, k * scaleHeight, j * scaleWidthDepth);

					// push back the normal vec3 for this point to faceNormals vector.	
					faceNormals.push_back(glm::normalize(glm::cross(directions[2] - directions[3], directions[0] - directions[1])));
				}
			}
		}

		// unload the image.
		SOIL_free_image_data(image);

		// calculate tangent
		glm::vec3 faceTangent = glm::vec3(1.0f, 0.0f, 0.0f);

		// push back values for triangles as long as i is less than points.size - depth and i is not a multiple of depth (not bottom or rightmost row).
		for (int i = 0; i < points.size() - depth; i++)
		{
			if ((i+1) % depth != 0)
			{
				triangles.push_back(i);
				triangles.push_back(i + 1);
				triangles.push_back(i + depth);
				triangles.push_back(i + 1);
				triangles.push_back(i + 1 + depth);
				triangles.push_back(i + depth);
			}
		}

		// push back values to create geometry, we also push back values for the texture UVs and the splatmap UVs.
		for (int i = 0; i < triangles.size(); i++)
		{
			positions.push_back(points[triangles[i]]);
			normals.push_back(faceNormals[triangles[i]]);
			tangents.push_back(faceTangent);
			uvs.push_back(glm::vec2(positions[i].x * 0.1f,positions[i].z * 0.1f));

			// splat UVs will allow the splatmap to stretch accross the entire terrain.
			splatUVs.push_back(glm::vec2(positions[i].x / (width*scaleWidthDepth), positions[i].z / (width*scaleWidthDepth)));
		}
	}

public: 
	TerrainGeometry(const char* directory, float scaleHeight, float scaleWidthDepth) { CreateGeometry(directory, scaleHeight, scaleWidthDepth); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec2>& GetSplatUVVector() const { return splatUVs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }

	// returns closest point on the terrain to a given position.
	glm::vec3 returnClosestNode(glm::vec3 currentPosition)
	{
		int i = currentPosition.x / ScaleWidthDepth;
		int j = currentPosition.z / ScaleWidthDepth;

		// if i or j go out of range we reset them.
		if (i < 0)
		{
			i = 0;
		}
		else if (i > width - 1)
		{
			i = width - 1;
		}
		if (j < 0)
		{
			j = 0;
		}
		else if (j > depth - 1)
		{
			j = depth - 1;
		}

		// returns the vector3 from points at the index of i*width+j.
		return points[i*width + j];
	}
	int GetNumVertices() const { return positions.size(); }
};
