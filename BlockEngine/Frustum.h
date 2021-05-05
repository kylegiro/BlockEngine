#pragma once

#include "Plane.h"

#include <string>

class Frustum
{
public:
	Frustum(glm::mat4 projection);
	bool pointInFrustum(glm::vec3 point);
	bool cubeInFrustum(glm::vec3 center, float size);
	std::string toString();

public:
	enum PlaneType {
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_BOTTOM,
		PLANE_TOP,
		PLANE_FAR,
		PLANE_NEAR
	};

	enum Halfspace {
		NEGATIVE,
		ON_PLANE,
		POSITIVE
	};

private:
	Plane planes[6];

};

