#pragma once

#include "ScreenTransform.h"
#include "Mat.h"
#include "Graphics.h"
#include "Geometry.h"


#include "Color.h"

template<class Vertex>
class Pipeline {
public:
	Pipeline(Graphics &gfx, ScreenTransform &sst) : gfx(gfx), sst(sst) {}

	void Draw(const IndexedTriangleList<Vertex>& it) {
		//Process vertices
		//std::vector<Vertex> verticesOut(vertices.size());
		//std::transform(it.vertices.begin(), it.vertices.end(), 
		//	verticesOut.begin(), transformFn)

		// assemble triangles
		for (size_t i = 0, end = it.indices.size() / 3; i < end; i++) {
			const auto& v0 = it.vertices[it.indices[i * 3]];
			const auto& v1 = it.vertices[it.indices[i * 3 + 1]];
			const auto& v2 = it.vertices[it.indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				float dot = ((v1.pos - v0.pos) % (v2.pos - v1.pos)).GetNormalized() * v0.pos.GetNormalized();
				ProcessTriangle(v0, v1, v2, i, -dot);

			}
		}
	}

	void Draw(const IndexedTriangleList<Vertex>& it, Vec3 light) {
		//Process vertices
		//std::vector<Vertex> verticesOut(vertices.size());
		//std::transform(it.vertices.begin(), it.vertices.end(), 
		//	verticesOut.begin(), transformFn)

		// assemble triangles
		for (size_t i = 0, end = it.indices.size() / 3; i < end; i++) {
			const auto& v0 = it.vertices[it.indices[i * 3]];
			const auto& v1 = it.vertices[it.indices[i * 3 + 1]];
			const auto& v2 = it.vertices[it.indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				float dot = ((v1.pos - v0.pos) % (v2.pos - v1.pos)).GetNormalized() * light.GetNormalized();
				if (end == 12)
					ProcessTriangle(v0, v1, v2, i, -dot, true);
				else 
					ProcessTriangle(v0, v1, v2, i, -dot);


			}
		}
	}

	void ProcessTriangle(const Vertex &v00, const  Vertex &v10, const Vertex &v20, size_t triangle_index, float dot = 1.0f, bool cube = false) {
		// GS returns triangle which has copies of vertexs
		Vec3 v0 = v00.pos;
		Vec3 v1 = v10.pos;
		Vec3 v2 = v20.pos;
		// cull + clip

		// 
		sst.TransformSphere(v0);
		sst.TransformSphere(v1);
		sst.TransformSphere(v2);

		// take into account borders
		// assumes that we won't be drawing triangles wider than 32 in x axis around borders
		float _max = 0;
		if (v0.x > v1.x and v0.x > v2.x) 
			_max = v0.x;
		else if (v1.x > v0.x and v1.x > v2.x) 
			_max = v1.x;
		else 
			_max = v2.x;

		if (_max - v0.x > SCREEN_WIDTH/2)
			v0.x += SCREEN_WIDTH;
		if (_max - v1.x > SCREEN_WIDTH / 2)
			v1.x += SCREEN_WIDTH;
		if (_max - v2.x > SCREEN_WIDTH / 2)
			v2.x += SCREEN_WIDTH;

		unsigned char hue;
		if (cube)
			hue = ((triangle_index / 2) * 40) % 256;
		else 
			hue = ((triangle_index / 16) * 5) % 256;

		CRGB colour;// = CHSV(hue, 255, 255);
		float al = max(min(int(255.0f * dot), 255), 64);
		colour.setHSV(hue, 255, al);
		Vec3 c;
		c.x = colour.r;
		c.y = colour.g;
		c.z = colour.b;

		gfx.drawTriangle(v0, v1, v2, c);

	}

	void ClipCullTriangle(Triangle<GSVertex>& t)
	{
		// cull tests
		if (t.v0.pos.y > 1.0f &&
			t.v1.pos.y > 1.0f &&
			t.v2.pos.y > 1.0f)
		{
			return;
		}
		if (t.v0.pos.y < -1.0f &&
			t.v1.pos.y < -1.0f &&
			t.v2.pos.y < -1.0f)
		{
			return;
		}

		// clipping routines
		const auto Clip1 = [this](GSVertex& v0, GSVertex& v1, GSVertex& v2)
		{
			// calculate alpha values for getting adjusted vertices
			const float alphaA = (-v0.pos.z) / (v1.pos.z - v0.pos.z);
			const float alphaB = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			// interpolate to get v0a and v0b
			const auto v0a = interpolate(v0, v1, alphaA);
			const auto v0b = interpolate(v0, v2, alphaB);
			// draw triangles
			PostProcessTriangleVertices(Triangle<GSVertex>{ v0a, v1, v2 });
			PostProcessTriangleVertices(Triangle<GSVertex>{ v0b, v0a, v2 });
		};
		const auto Clip2 = [this](GSVertex& v0, GSVertex& v1, GSVertex& v2)
		{
			// calculate alpha values for getting adjusted vertices
			const float alpha0 = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			const float alpha1 = (-v1.pos.z) / (v2.pos.z - v1.pos.z);
			// interpolate to get v0a and v0b
			v0 = interpolate(v0, v2, alpha0);
			v1 = interpolate(v1, v2, alpha1);
			// draw triangles
			PostProcessTriangleVertices(Triangle<GSVertex>{ v0, v1, v2 });
		};


		// near clipping tests
		PostProcessTriangleVertices(t);
	}

	void PostProcessTriangleVertices(Triangle<GSVertex> t) {
		// GS returns triangle which has copies of vertexs
		// cull + clip

		// 
		sst.TransformSphere(t.v0.pos);
		sst.TransformSphere(t.v1.pos);
		sst.TransformSphere(t.v2.pos);




		gfx.drawTriangle(t.v0.pos, t.v1.pos, t.v2.pos, t.v0.col);

	}


private:
	Graphics &gfx;
	ScreenTransform &sst;
};

// receives indexed list of vertices pre transformed (i.e rotated/translated in world space)
// vertex shader

// transformation

// tessellation

//geometrie shader

// transform to screen space

// backface culling and clipping

// rasterise 

// early z
// pixel shader