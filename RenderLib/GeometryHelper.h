#pragma once
//
// Abstract base class for Geometry helpers (things which provide the
// vertex data for a mesh 
//
#include "stdafx.h"
#include <vector>

class GeometryHelper
{
public:
	virtual const std::vector<glm::vec3>& GetPositionVector() const = 0;
	virtual const std::vector<glm::vec3>& GetNormalVector() const = 0;
	virtual const std::vector<glm::vec2>& GetUVVector() const = 0;
	virtual const std::vector<glm::vec3>& GetTangentVector() const = 0;
	virtual int GetNumVertices() const = 0;
};