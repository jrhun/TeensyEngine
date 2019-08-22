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
		sst.TransformSphere(v0);
		sst.TransformSphere(v1);
		sst.TransformSphere(v2);

		// take into account borders
		// assumes that we won't be drawing triangles wider than 32 in x axis around borders
		bool oneGreater = false;
		const float lower = SCREEN_WIDTH / 4;
		const float upper = 3 * SCREEN_WIDTH / 4;



		if (v0.x > upper || v1.x > upper || v2.x > upper) {
			//oneGreater = true;
			//if (oneGreater) {
				if (v0.x < lower + 1)
					v0.x += SCREEN_WIDTH;
				if (v1.x < lower + 1)
					v1.x += SCREEN_WIDTH;
				if (v2.x < lower + 1)
					v2.x += SCREEN_WIDTH;
			//}
		}

		if (abs(v0.x - v1.x) > SCREEN_WIDTH / 2) {
			// HOW DID WE GET HERE??
			DebugBreak();
		}
		else if (abs(v2.x - v1.x) > SCREEN_WIDTH / 2) {
			// HOW DID WE GET HERE??
			DebugBreak();
		}
		else if (abs(v2.x - v0.x) > SCREEN_WIDTH / 2) {
			// HOW DID WE GET HERE??
			DebugBreak();
		}



		unsigned char hue = ((triangle_index / 2) * 40) %256;
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

		CRGB colour;// = CHSV(hue, 255, 255);
		colour.setHSV(hue, 255, 255);
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