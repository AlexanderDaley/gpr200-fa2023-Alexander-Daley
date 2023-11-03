#include "procGen.h"

namespace AOD {
	//ew::MeshData createSphere(float radius, int numSegments) {

	//}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		float topY = height / 2;
		float bottomY = -topY;
		ew::Vertex v;
		//front center
		v.pos = ew::Vec3(0, 0, topY);
		v.normal = ew::Vec3(0.0f, 0.0f, 1.0f);
		mesh.vertices.push_back(v);
		//Back center
		v.pos = ew::Vec3(0, 0, bottomY);
		v.normal = ew::Vec3(0.0f, 0.0f, -1.0f);
		mesh.vertices.push_back(v);
		float thetaStep = 2*3.1415926535 / numSegments;
		//Front cap (Forward normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = topY;
			v.normal = ew::Vec3(0.0f, 0.0f, 1.0f);
			mesh.vertices.push_back(v);
		}
		//back cap (Forward normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = bottomY;
			v.normal = ew::Vec3(0.0f, 0.0f, -1.0f);
			mesh.vertices.push_back(v);
		}
		//Front cap (side normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = topY;
			v.normal = ew::Vec3(1.0f, 0.0f, 0.0f);
			mesh.vertices.push_back(v);
		}
		//back cap (side normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = bottomY;
			v.normal = ew::Vec3(-1.0f, 0.0f, 0.0f);
			mesh.vertices.push_back(v);
		}
		int start = 2;
		int center = 0;
		//front indices
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(start + i + 1);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i);
		}
		center = 1;
		start = 3 + numSegments;
		//back indices
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
		}
		int sideStart = 4 + numSegments * 2;
		int columns = numSegments + 1;
		//Side indices
		for (int i = 0; i < columns - 1; i++) {
			start = sideStart + i;
			//Triangle 1
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start);
			//Triangle 2
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + columns + 1);
		}

		return(mesh);
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