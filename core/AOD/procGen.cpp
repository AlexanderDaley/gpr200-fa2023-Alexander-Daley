#include "procGen.h"

namespace AOD {
	//ew::MeshData createSphere(float radius, int numSegments) {

	//}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		int topY = height / 2;
		int bottomY = -topY;
		ew::Vertex v;
		v.pos = ew::Vec3(0, topY, 0);
		mesh.vertices.push_back(v);
		thetaStep = 2PI / numSegments;

		for (int i = 0; i <= numSegments; i++) {
			int theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = topY;
			mesh.vertices.push_back(v);
		}

	}

	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::MeshData mesh;
		int columns = subdivisions + 1;

		for (int row = 0; row <= subdivisions; row++) {
			for (int col = 0; col <= subdivisions; col++) {
				ew::Vertex v;
				v.pos.x = width * ((float)col / subdivisions);
				v.pos.z = -height * ((float)row / subdivisions);
				v.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
				v.uv = ew::Vec2(1-(float)row/subdivisions, (float)col/subdivisions);
				mesh.vertices.push_back(v);
			}
		}

		for (int row = 0; row < subdivisions; row++) {
			for (int col = 0; col < subdivisions; col++) {
				int start = row * columns + col;
				//Bottom right triangle
				mesh.indices.push_back(start);
				mesh.indices.push_back(start+1);
				mesh.indices.push_back(start+columns+1);
				//Top left triangle
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
			}
		}

		return(mesh);
	}
}