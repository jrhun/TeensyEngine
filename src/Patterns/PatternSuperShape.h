#pragma once

#include "Pattern.h"
#include "../../data.h"
#include "../Geometry.h"
#include "../Cube.h"

class PatternSuperShape : public Pattern {
public:
	PatternSuperShape() : Pattern("Super Shape") {
		cube = getVerts<Vertex>(0, 1.1f);
		sphere = getVerts<Vertex>(1, 1.1f);
	}

	float cameraOffset = 0.0;
	float offsetDir = 1.0;
	float angle = 0;
	int shape1 = 0;

	IndexedTriangleList<Vertex> cube;
	IndexedTriangleList<Vertex> sphere;

	void start() {

	}

	uint8_t drawFrame() {
		if (Pattern::useDefaultEffect) {
			gfx.fade(128);
		}

		engine.resetZ();

		//int shape2 = (shape1 + 1) % numShapes;
		//static float s = 0;


		//auto it1 = getVerts<Vertex>(0, 1.1f);
		//auto it2 = getVerts<Vertex>(1, 1.7f);

		auto it1 = cube;
		auto it2 = sphere;

		Vec3 light = {
			float((cameraOffset + 3) * cos(angle + PI)),
			0.0f,
			float((cameraOffset + 3) * sin(angle + PI))
		};

		Mat3 m = Mat3::RotationX(angle)  * Mat3::RotationY(angle);
		for (auto &v : it1.vertices) {
			v.pos = v.pos * m;
			v.pos += light;
		}

		//engine.pipeline.Draw(it1, light);


		for (auto &v : it2.vertices) {
			/*Mat3 m = Mat3::RotationX(angle)  * Mat3::RotationY(angle);*/
			v.pos = v.pos * m;
			v.pos += light;
		}

		//smoothly interpolated between two shapes
		int beat = beatsin16(5, 0, 2048);
		float inter =  beat / 2048.0f;
		for (unsigned int i = 0; i < it1.vertices.size(); i++) {
			it1.vertices[i].pos = interpolate(it1.vertices[i].pos, it2.vertices[i].pos, inter);
		}
		engine.pipeline.Draw(it1, light);

		//// flip position
		for (auto &v : it1.vertices) {
			v.pos.x = -v.pos.x;
			v.pos.z = -v.pos.z;
		}
		light.x = -light.x;
		light.z = -light.z;
		engine.pipeline.Draw(it1, light);


		if (cameraOffset > 1)
			offsetDir = -1.0f;
		if (cameraOffset < -1)
			offsetDir = 1.0f;
		cameraOffset += 0.01 * offsetDir;

		angle += 0.01;
		if (angle >= TWO_PI)
			angle -= TWO_PI;

		return returnVal;
	}

	static float supershape(float theta, float m, float n1, float n2, float n3) {
		float a = 1;
		float b = 1;
		float t1 = abs((1 / a) * cos(m * theta / 4));
		t1 = pow(t1, n2);
		float t2 = abs((1 / b) * sin(m * theta / 4));
		t2 = pow(t2, n3);
		float t3 = t1 + t2;
		float r = pow(t3, -1 / n1);

		return r;
	}

	template<class V>
	IndexedTriangleList<V> getVerts(int shapeIndex, float radius = 1.0f) {
		std::vector<V> vertices;
		std::vector<uint16_t> indices;
		const int latDiv = 8;
		const int longDiv = 16;

		const Vec3 base = { 0.0f, radius, 0.0f };
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		for (int lat = 1; lat < latDiv; lat++) {
			//const auto latBase = base * Mat3::RotationZ(lattitudeAngle * lat);
			float latA = lattitudeAngle * lat - PI / 2;
			//float r2 = supershape(latA, 1, 10,10,10);  //nice shape
			//float r2 = supershape(latA, 4, 100, 100, 100); //cube
			float r2 = shapes[shapeIndex].r2(latA);
			for (int lon = 0; lon < longDiv; lon++) {
				float lonA = longitudeAngle * lon - PI;
				//float r1 = supershape(lonA, 8, 60, 100, 30);
				//float r1 = supershape(lonA, 4, 100, 100, 100); //cube
				float r1 = shapes[shapeIndex].r1(lonA);
				float x = radius * r1 * cos(lonA) * r2 * cos(latA);
				float z = radius * r1 * sin(lonA) * r2 * cos(latA);
				float y = radius * r2 * sin(latA);


				vertices.emplace_back();
				//vertices.back().pos = latBase * Mat3::RotationY(longitudeAngle * lon);
				Vec3 p = Vec3(x, y, z);
				vertices.back().pos = p;// { x, y, z };
			}
		}
		// add the cap vertices
		const auto iNorthPole = vertices.size();
		vertices.emplace_back();
		vertices.back().pos = base;
		const auto iSouthPole = vertices.size();
		vertices.emplace_back();
		vertices.back().pos = -base;

		const auto calcIdx = [latDiv, longDiv](int iLat, int iLong)
		{ return iLat * longDiv + iLong; };

		for (int iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (int iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}
		// cap fans
		for (int iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return{ std::move(vertices),std::move(indices) };
	}

	struct Shape {
		float r1_m;
		float r1_n1;
		float r1_n2;
		float r1_n3;
		float r2_m;
		float r2_n1;
		float r2_n2;
		float r2_n3;

		float r1(float theta) {
			return supershape(theta, r1_m, r1_n1, r1_n2, r1_n3);
		}
		float r2(float theta) {
			return supershape(theta, r2_m, r2_n1, r2_n2, r2_n3);
		}
	};

	static const int numShapes = 4;
	Shape shapes[numShapes] = {
		{4, 100, 100, 100, 4, 100, 100, 100}, // cube
		{0, 1, 1, 1, 0, 1, 1, 1}, // sphere
		{8, 60, 100, 30, 2, 10, 10, 10}, //seedling
		{4, 1, 1, 1, 4, 1, 1, 1} // diamond
	};


};