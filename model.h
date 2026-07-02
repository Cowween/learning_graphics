#pragma once
#include <vector>
#include "geometry.h"
#include <string>


class Model {

	std::vector<vec3> verts = {};
	std::vector<int> face_verts = {};

public:

	Model(const std::string filename);
	int getVertCount() const;
	int getFacesCount() const;
	vec3 vert(const int i) const;
	vec3 vert(const int iface, const int nvert) const;
};