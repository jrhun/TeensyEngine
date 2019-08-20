#pragma once

#include "Vec3.h"

struct Vertex {
	Vertex() {}
	Vertex(Vec3 &v) : pos(v) {}

	Vec3 pos;
};

struct GSVertex : public Vertex {
public:
	GSVertex(Vertex &v, Vec3 &c) : Vertex(v), col(c) {}

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