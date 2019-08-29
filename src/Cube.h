#pragma once

#include <vector>
#include "Geometry.h"

class Cube {
public:
	Cube() {}

	static IndexedLineList getLines(float size = 1.0f) {
		const float side = size / 2.0f;

		std::vector<Vec3> vertices;

		vertices.emplace_back(-side, -side, -side); // 0
		vertices.emplace_back(side, -side, -side); // 1
		vertices.emplace_back(-side, side, -side); // 2
		vertices.emplace_back(side, side, -side); // 3
		vertices.emplace_back(-side, -side, side); // 4
		vertices.emplace_back(side, -side, side); // 5
		vertices.emplace_back(-side, side, side); // 6
		vertices.emplace_back(side, side, side); // 7

		return { 
			std::move(vertices), {
				0,1,	1,3,	3,2,	2,0,
				0,4,	1,5,	3,7,	2,6,
				4,5,	5,7,	7,6,	6,4}
		};
	}

	template<class V>
	static IndexedTriangleList<V> getTriangles(float size = 1.0f)
	{
		const float side = size / 2.0f;

		std::vector<Vec3> vertices;

		vertices.emplace_back(-side, -side, -side); // 0
		vertices.emplace_back(side, -side, -side); // 1
		vertices.emplace_back(-side, side, -side); // 2
		vertices.emplace_back(side, side, -side); // 3
		vertices.emplace_back(-side, -side, side); // 4
		vertices.emplace_back(side, -side, side); // 5
		vertices.emplace_back(-side, side, side); // 6
		vertices.emplace_back(side, side, side); // 7

		std::vector<V> verts(8); // teensyduino does't work if we pass size_t to it
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}

		return{
			std::move(verts),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}


};

