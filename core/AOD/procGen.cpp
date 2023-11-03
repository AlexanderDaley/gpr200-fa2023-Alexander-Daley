#include "procGen.h"

namespace AOD {
	ew::MeshData createSphere(float radius, int numSegments) {
		float thetaStep = 2 * 3.1415926535 / numSegments;
		float phiStep = 3.1415926535 / numSegments;
		float phi, theta;
		ew::Vertex v;
		ew::MeshData mesh;

		//Set vertices
		for (int row = 0; row <= numSegments; row++) {
			phi = row * phiStep;
			for(int col = 0; col <= numSegments; col++){
				theta = col * thetaStep;
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);
				v.normal = ew::Normalize(v.pos);
				v.uv = ew::Vec2(1 - (float)row / numSegments, (float)col / numSegments);
				mesh.vertices.push_back(v);
				
			}
		}

		int poleStart = 0;
		int sideStart = numSegments + 1;
		//Set top cap indices
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(sideStart + i);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i + 1);
		}
		
		int columns = numSegments + 1;
		int start;
		//Set row indices
		for (int row = 1; row < numSegments - 1; row++) {
			for (int col = 0; col < numSegments; col++) {
				start = row * columns + col;
				//First triangle
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns);
				//Second triangle
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start + 1);
			}
		}

		poleStart = (numSegments + 1)*(numSegments) + 1;
		sideStart = (numSegments + 1)*(numSegments) - numSegments - 1;
		//Set bottom cap indices
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(sideStart + i + 1);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i);
		}

		return(mesh);
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		float topY = height / 2;
		float bottomY = -topY;
		ew::Vertex v;
		//front center
		v.pos = ew::Vec3(0, 0, topY);
		v.normal = ew::Vec3(0.0f, 0.0f, 1.0f);
		v.uv = ew::Vec2(0.5f, 0.5f);
		mesh.vertices.push_back(v);
		//Back center
		v.pos = ew::Vec3(0, 0, bottomY);
		v.normal = ew::Vec3(0.0f, 0.0f, -1.0f);
		v.uv = ew::Vec2(0.5f, 0.5f);
		mesh.vertices.push_back(v);
		float thetaStep = 2*3.1415926535 / numSegments;
		//Front cap (Forward normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = topY;
			v.normal = ew::Vec3(0.0f, 0.0f, 1.0f);
			v.uv = ew::Vec2((sin(theta)+1) / 2,(cos(theta)+1)/2);
			mesh.vertices.push_back(v);
		}
		//back cap (Forward normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = bottomY;
			v.normal = ew::Vec3(0.0f, 0.0f, -1.0f);
			v.uv = ew::Vec2((-sin(theta) + 1) / 2, (-cos(theta) + 1) / 2);
			mesh.vertices.push_back(v);
		}
		//Front cap (side normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = topY;
			v.normal = ew::Vec3(cos(theta), sin(theta), 0.0f);
			v.uv = ew::Vec2((cos(theta) + 1) / 2, 1);
			mesh.vertices.push_back(v);
		}
		//back cap (side normals)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.y = sin(theta) * radius;
			v.pos.z = bottomY;
			v.normal = ew::Vec3(cos(theta), sin(theta), 0.0f);
			v.uv = ew::Vec2((cos(theta) + 1) / 2, 0);
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