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
			return(AOD::Perspective(ew::Radians(fov), aspectRatio, nearPlane, farPlane));
		}
	}

	void CameraControls::moveCamera(GLFWwindow* window, Camera* camera, CameraControls* controls, float deltaTime) {
		//If right mouse is not held, release cursor and return early.
		if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
			//Release cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			controls->firstMouse = true;
			return;
		}
		//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse.
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Get screen mouse position this frame
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//If we just started right clicking, set prevMouse values to current position.
		//This prevents a bug where the camera moves as soon as we click.
		if (controls->firstMouse) {
			controls->firstMouse = false;
			controls->prevMouseX = mouseX;
			controls->prevMouseY = mouseY;
		}

		pitch -= mouseSensitivity*(mouseY - prevMouseY);
		yaw += mouseSensitivity*(mouseX - prevMouseX);
		if (pitch > 89) {
			pitch = 89;
		}
		else if (pitch < -89) {
			pitch = -89;
		}

		float pitchRad = ew::Radians(pitch);
		float yawRad = ew::Radians(yaw);

		ew::Vec3 forward = ew::Vec3 (sin(yawRad) * cos(pitchRad), sin(pitchRad), -cos(yawRad) * cos(pitchRad));

		ew::Vec3 right = ew::Normalize(ew::Cross(ew::Vec3(0,1,0), forward));
		ew::Vec3 up = ew::Normalize(ew::Cross(forward, right));

		//Key controls
		if (glfwGetKey(window, GLFW_KEY_W)) {
			camera->position += forward * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			camera->position += right * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			camera->position -= forward * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			camera->position -= right * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_Q)) {
			camera->position -= up * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_E)) {
			camera->position += up * controls->moveSpeed * deltaTime;
		}

		camera->target = camera->position + forward;
		//Remember previous mouse position
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	void Camera::resetCamera() {
		position = ew::Vec3(0, 0, 5);
		target = ew::Vec3(0, 0, 0);
	}

	void CameraControls::resetControls() {
		yaw = 0;
		pitch = 0;
	}
}
