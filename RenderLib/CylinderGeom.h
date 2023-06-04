#pragma once
#pragma once

#include "GeometryHelper.h"

class CylinderGeometry : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateGeometry(int nDivisions, float height)
	{
		glm::vec3 *circle = new glm::vec3[nDivisions];
		for (int i = 0; i < nDivisions; i++)
		{
			float theta = glm::radians(360.0) * (float)i / (nDivisions - 1);
			circle[i] = glm::vec3(cos(theta), 0.0f, sin(theta));
		}
		// make the sides - nDivisions quads
		glm::vec3 h = glm::vec3(0, height, 0);

		for (int i = 0; i < nDivisions; i++)
		{
			glm::vec3 n[] = { circle[i], circle[i], circle[(i + 1)%nDivisions], circle[(i + 1)%nDivisions] };
			glm::vec3 r[] = { circle[i] - h, circle[i] + h, circle[(i + 1)%nDivisions] + h, circle[(i+1)%nDivisions] - h };
			glm::vec2 u[] = { glm::vec2((float)i / nDivisions,0), glm::vec2((float)i / nDivisions,1),
							  glm::vec2((float)(i + 1) / nDivisions, 1), glm::vec2((float)(i + 1) / nDivisions,0) };
			glm::vec3 t[] = { glm::cross(n[0],glm::vec3(0,1,0)),glm::cross(n[1],glm::vec3(0,1,0)), glm::cross(n[2],glm::vec3(0,1,0)), glm::cross(n[3],glm::vec3(0,1,0)) };

			positions.push_back(r[0]); positions.push_back(r[1]); positions.push_back(r[2]); 
			positions.push_back(r[0]); positions.push_back(r[2]); positions.push_back(r[3]);

			normals.push_back(n[0]); normals.push_back(n[1]); normals.push_back(n[2]);
			normals.push_back(n[0]); normals.push_back(n[2]); normals.push_back(n[3]);

			tangents.push_back(t[0]); tangents.push_back(t[1]); tangents.push_back(t[2]);
			tangents.push_back(t[0]); tangents.push_back(t[2]); tangents.push_back(t[3]);

			uvs.push_back(u[0]); uvs.push_back(u[1]); uvs.push_back(u[2]);
			uvs.push_back(u[0]); uvs.push_back(u[2]); uvs.push_back(u[3]);
		}

	// make the top and bottom
		
		// top
		glm::vec3 n = glm::vec3(0, 1, 0);
		glm::vec3 t = glm::vec3(1, 0, 0);
		for (int i = 0; i < nDivisions-1; i++)
		{
			positions.push_back(circle[0]+h); positions.push_back(circle[i + 1]+h), positions.push_back(circle[(i + 2) % nDivisions]+h);
			normals.push_back(n); normals.push_back(n); normals.push_back(n);
			tangents.push_back(t); tangents.push_back(t); tangents.push_back(t);
			uvs.push_back(glm::vec2(circle[0].x, circle[0].z)*0.5f);
			uvs.push_back(glm::vec2(circle[i+1].x, circle[i+1].z)*0.5f);
			uvs.push_back(glm::vec2(circle[(i+2)%nDivisions].x, circle[(i+2)%nDivisions].z)*0.5f);
		}
		// bottom


	}
public:
	CylinderGeometry(int nDivisions, float height) { CreateGeometry(nDivisions, height); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

};
