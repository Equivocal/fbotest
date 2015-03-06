#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/glew.h>
#include <string>
#include <glm/vec2.hpp>
#include <array>
#include <vector>

namespace nex {
	enum GeometryType { GEO_OCCLUDER, GEO_DEFAULT, GEO_COLLIDABLE };

	struct Vertex {
		glm::vec2 p;
		glm::vec2 t;
		float z;
		GLuint tex;
		GeometryType type;
	};

	struct Edge {
		Vertex v;
		glm::vec2 d;
		glm::vec2 normal;
	};

	struct Quad {
		std::string texture;
		std::string atlas;
		std::array<Edge, 4> edges;
		float width;
		float height;
		float z;
		GeometryType type;

		std::vector<Vertex> vertices() {
			std::vector<Vertex> ret;
			for (Edge e : edges) {
				ret.push_back(e.v);
			}
			return ret;
		}
	};
}

#endif