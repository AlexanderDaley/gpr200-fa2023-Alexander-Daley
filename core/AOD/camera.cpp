#include "camera.h"
#include "transformations.h"

namespace AOD {
	ew::Mat4 Camera::ViewMatrix() {
		return(AOD::LookAt(position, target, ew::Vec3(0, 1, 0)));
	}

	ew::Mat4 Camera::ProjectionMatrix() {
		if (orthographic) {
			return(AOD::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane));
		}
		else {
			return(AOD::Perspective(fov, aspectRatio, nearPlane, farPlane));
		}
	}
}
