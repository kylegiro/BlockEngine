#include "Frustum.h"

#include <sstream>

// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
// glm matrix element access is mat[column][row]

Frustum::Frustum(glm::mat4 projection)
{	
	float a = projection[0][3] + projection[0][0]; // m_41 + m_11
	float b = projection[1][3] + projection[1][0]; // m_42 + m_12
	float c = projection[2][3] + projection[2][0]; // m_43 + m_13
	float d = projection[3][3] + projection[3][0]; // m_44 + m_14

	Plane planeLeft = Plane(a, b, c, d);
	planes[PLANE_LEFT] = planeLeft;

	a = projection[0][3] - projection[0][0];
	b = projection[1][3] - projection[1][0];
	c = projection[2][3] - projection[2][0];
	d = projection[3][3] - projection[3][0];

	Plane planeRight = Plane(a, b, c, d);
	planes[PLANE_RIGHT] = planeLeft;

	a = projection[0][3] + projection[0][1];
	b = projection[1][3] + projection[1][1];
	c = projection[2][3] + projection[2][1];
	d = projection[3][3] + projection[3][1];

	Plane planeBottom = Plane(a, b, c, d);
	planes[PLANE_BOTTOM] = planeBottom;

	a = projection[0][3] - projection[0][1];
	b = projection[1][3] - projection[1][1];
	c = projection[2][3] - projection[2][1];
	d = projection[3][3] - projection[3][1];

	Plane planeTop = Plane(a, b, c, d);
	planes[PLANE_TOP] = planeTop;

	a = projection[0][3] - projection[0][2];
	b = projection[1][3] - projection[1][2];
	c = projection[2][3] - projection[2][2];
	d = projection[3][3] - projection[3][2];

	Plane planeNear = Plane(a, b, c, d);
	planes[PLANE_NEAR] = planeNear;

	a = projection[0][3] + projection[0][2];
	b = projection[1][3] + projection[1][2];
	c = projection[2][3] + projection[2][2];
	d = projection[3][3] + projection[3][2];

	Plane planeFar = Plane(a, b, c, d);
	planes[PLANE_FAR] = planeFar;
}

bool Frustum::pointInFrustum(glm::vec3 point)
{
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].distance(point) <= 0)
			return false;
	}
	return true;
}

bool Frustum::cubeInFrustum(glm::vec3 center, float size)
{
	// check center first
	if (pointInFrustum(center))
		return true;
	// then corners
	if (pointInFrustum(center + glm::vec3(size, size, size)))
		return true;
	if (pointInFrustum(center + glm::vec3(-size, size, size)))
		return true;
	if (pointInFrustum(center + glm::vec3(size, -size, size)))
		return true;
	if (pointInFrustum(center + glm::vec3(size, size, -size)))
		return true;
	if (pointInFrustum(center + glm::vec3(-size, -size, size)))
		return true;
	if (pointInFrustum(center + glm::vec3(size, -size, -size)))
		return true;
	if (pointInFrustum(center + glm::vec3(-size, size, -size)))
		return true;
	if (pointInFrustum(center + glm::vec3(-size, -size, -size)))
		return true;

	return false;
}

std::string Frustum::toString()
{
	std::ostringstream ss;
	
	for (int i = 0; i < 6; i++)
	{
		ss << planes[i].toString() << std::endl;
	}

	return ss.str();	
}
