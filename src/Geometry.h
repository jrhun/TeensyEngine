#pragma once

#include "Vec3.h"
#include <vector>

struct Vertex {
	Vertex() {}
	Vertex(Vec3 &v) : pos(v) {}
	Vertex(const Vec3 &v) : pos(v) {}

	Vec3 pos;
};

struct GSVertex : public Vertex {
public:
	GSVertex() {}
	GSVertex(Vertex &v, Vec3 &c) : Vertex(v), col(c) {}
	//GSVertex(Vec3 &v, Vec3 &c) : Vertex(v), col(c) {}
	//GSVertex(Vertex& pos) : Vertex(pos) {}
	//GSVertex(Vec3& pos) : Vertex(pos) {}
	//GSVertex(GSVertex &v, GSVertex &c) : Vertex(v.pos), col(c.col) {}
	//GSVertex(GSVertex &v) : Vertex(v.pos), col(v.col) {}
	//GSVertex(const GSVertex &v) : Vertex(v.pos), col(v.col) {}
	GSVertex(const Vertex &v, Vec3 &c) : col(c) {
		pos = v.pos;
	}
	GSVertex(const Vertex &v) {
		pos = v.pos;
	}

	//GSVertex& operator+=(const GSVertex& rhs)
	//{
	//	pos += rhs.pos;
	//	return *this;
	//}
	//GSVertex operator+(const GSVertex& rhs) const
	//{
	//	return GSVertex(*this) += rhs;
	//}
	//GSVertex& operator-=(const GSVertex& rhs)
	//{
	//	pos -= rhs.pos;
	//	return *this;
	//}
	//GSVertex operator-(const GSVertex& rhs) const
	//{
	//	return GSVertex(*this) -= rhs;
	//}
	//GSVertex& operator*=(float rhs)
	//{
	//	pos *= rhs;
	//	return *this;
	//}
	//GSVertex operator*(float rhs) const
	//{
	//	return GSVertex(*this) *= rhs;
	//}
	//GSVertex& operator/=(float rhs)
	//{
	//	pos /= rhs;
	//	return *this;
	//}
	//GSVertex operator/(float rhs) const
	//{
	//	return GSVertex(*this) /= rhs;
	//}

	Vec3 col;
};

class IndexedLineList {
public:
	IndexedLineList() = default;
	IndexedLineList(std::vector<Vec3> verts_in, std::vector<size_t> indices_in) :
		vertices(std::move(verts_in)),
		indices(std::move(indices_in)) {}

	std::vector<Vec3> vertices;
	std::vector<size_t> indices;
};

template<class T>
class IndexedTriangleList {
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> verts_in, std::vector<size_t> indices_in) :
		vertices(std::move(verts_in)),
		indices(std::move(indices_in)) {}

	std::vector<T> vertices;
	std::vector<size_t> indices;
};

template<class V>
class Triangle
{
public:
	Triangle(V &a, V &b, V &c) : v0(a), v1(b), v2(c) {}
	Triangle(const V &a, const V &b, const V &c) : v0(a), v1(b), v2(c) {
		v0 = a; v1 = b; v2 = c;
	}
	V v0;
	V v1;
	V v2;
};

template<class Vertex>
class DefaultGeometryShader
{
public:
	typedef Vertex Output;
public:
	Triangle<Output> operator()(const Vertex& in0, const Vertex& in1, const Vertex& in2, size_t triangle_index) const
	{
		return{ in0,in1,in2 };
	}
};