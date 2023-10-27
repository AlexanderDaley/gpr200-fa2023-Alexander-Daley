#include "procGen.h"

namespace AOD {
	//ew::MeshData createSphere(float radius, int numSegments) {

	//}

	//ew::MeshData createCylinder(float height, float radius, int numSegments) {
		
	//}

	ew::MeshData createPlane(float width, float height, int subdivisions) {
		for (int row = 0; row <= subdivisions; row++) {
			for (int col = 0; col <= subdivisions; col++) {
				v.x = width * (col / subdivisions);
				v.z = -height * (row / subdivisions);
				vertices.push_back(v);
			}
		}
	}
}