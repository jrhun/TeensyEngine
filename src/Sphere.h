#pragma once

#include "Geometry.h"
#include "Vec3.h"
#include <vector>

class Sphere {
public:
	Sphere() {}

	IndexedTriangleList<Vertex> getTriangles() {

		std::vector<Vertex> vertices;




		std::vector<size_t> indices;

		return { vertices, indices };


	}
};

void generateSphere() {
	for (int i = 0; i < 16; i++) {
		float lat = myMap(i, 0, 16, -PI / 2, PI / 2);
		for (int j = 0; j < 8; j++) {
			float lon = myMap(j, 0, 8, -PI, PI);
			const float r = 1;
			float x = r * sin(lon) * cos(lat);
			float y = r * sin(lon) * sin(lat);// j / 8.0f - 0.5f;
			float z = r * cos(lon);
			Vec3 v(x, y, z);
		}
	}
}