#pragma once

#include "ScreenTransform.h"
#include "Mat.h"
#include "Graphics.h"
#include "Geometry.h"


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
				// process 3 vertices into a triangle
				ProcessTriangle(v0, v1, v2, i);
			}
		}
	}

	void ProcessTriangle(const Vertex &v00, const  Vertex &v10, const Vertex &v20, size_t triangle_index) {
		// GS returns triangle which has copies of vertexs
		Vec3 v0 = v00.pos;
		Vec3 v1 = v10.pos;
		Vec3 v2 = v20.pos;
		// cull + clip

		// 
		sst.Transform(v0);
		sst.Transform(v1);
		sst.Transform(v2);

		unsigned char hue = (triangle_index / 2) * 40;
		Vec3 c;
		switch (triangle_index / 2) {
		case 0:
			c = { 255,0,0 };
			break;
		case 1:
			c = { 255,255,0 };
			break;
		case 2:
			c = { 0,255,0 };
			break;
		case 3:
			c = { 0,255,255 };
			break;
		case 4:
			c = { 0,0,255 };
			break;
		case 5:
			c = { 255,0,255 };
			break;
		};


		gfx.drawTriangle(v0, v1, v2, c);

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