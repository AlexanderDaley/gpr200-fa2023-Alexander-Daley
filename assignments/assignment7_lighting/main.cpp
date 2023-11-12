#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

struct Material {
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
};

struct Light {
	ew::Transform transform;  //Struct transform
	ew::Vec3 color; //RGB
};

float prevTime;
int currentLights = 3;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize light array
	Light lights[3];
	lights[0].color = ew::Vec3(1.0f, 0.0f, 0.0f);
	lights[1].color = ew::Vec3(0.0f, 1.0f, 0.0f);
	lights[2].color = ew::Vec3(0.0f, 0.0f, 1.0f);

	//Initialize world material
	Material mat = { 0, 1, 1, 50 };

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	//Initialize shaders and textures
	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader lightShader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	ew::Transform lightSphereTransform1;
	ew::Transform lightSphereTransform2;
	ew::Transform lightSphereTransform3;
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);
	lightSphereTransform1.position = ew::Vec3(-1.5f, 1.5f, 0.0f);
	lightSphereTransform2.position = ew::Vec3(0.0f, 1.5f, 0.0f);
	lightSphereTransform3.position = ew::Vec3(1.5f, 1.5f, 0.0f);
	lightSphereTransform1.scale = ew::Vec3(0.5f, 0.5f, 0.5f);
	lightSphereTransform2.scale = ew::Vec3(0.5f, 0.5f, 0.5f);
	lightSphereTransform3.scale = ew::Vec3(0.5f, 0.5f, 0.5f);

	resetCamera(camera,cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setInt("currentLights", currentLights);

		//Draw shapes from shader
		shader.setFloat("_Material.ambientK", mat.ambientK);
		shader.setFloat("_Material.diffuseK", mat.diffuseK);
		shader.setFloat("_Material.specular", mat.specular);
		shader.setFloat("_Material.shininess", mat.shininess);
		shader.setVec3("camPos", camera.position);

		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		//Render point lights
		lights[0].transform = lightSphereTransform1;
		lights[1].transform = lightSphereTransform2;
		lights[2].transform = lightSphereTransform3;

		shader.setVec3("_Lights[0].position", lights[0].transform.position);
		shader.setVec3("_Lights[0].color", lights[0].color);

		shader.setVec3("_Lights[1].position", lights[1].transform.position);
		shader.setVec3("_Lights[1].color", lights[1].color);

		shader.setVec3("_Lights[2].position", lights[2].transform.position);
		shader.setVec3("_Lights[2].color", lights[2].color);

		//Use new shader for light sources
		lightShader.use();
		lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//Draw shapes from lightShader
		lightShader.setMat4("_Model", lightSphereTransform1.getModelMatrix());
		lightShader.setVec3("_Color", lights[0].color);
		sphereMesh.draw();

		lightShader.setMat4("_Model", lightSphereTransform2.getModelMatrix());
		lightShader.setVec3("_Color", lights[1].color);
		sphereMesh.draw();

		lightShader.setMat4("_Model", lightSphereTransform3.getModelMatrix());
		lightShader.setVec3("_Color", lights[2].color);
		sphereMesh.draw();

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			//ImGui settings for camera
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}
			//ImGui for setting materials
			if (ImGui::CollapsingHeader("World Materials")) {
				ImGui::DragFloat("Ambient Constant", &mat.ambientK, 0.1f,0.0f,1.0f);
				ImGui::DragFloat("Diffuse Constant", &mat.diffuseK, 0.1f,0.0f,1.0f);
				ImGui::DragFloat("Specular Constant", &mat.specular,0.1f,0.0f,1.0f);
				ImGui::DragFloat("Shininess", &mat.shininess);
			}
			//ImGui for setting light 1 position and color
			if (ImGui::CollapsingHeader("Light 1")) {
				ImGui::DragFloat3("Light Position", &lightSphereTransform1.position.x, 0.1f);
				ImGui::DragFloat3("Light Color", &lights[0].color.x, 0.1f,0.0f,1.0f);
			}
			//ImGui for setting light 2 position and color
			if (ImGui::CollapsingHeader("Light 2")) {
				ImGui::DragFloat3("Light Position", &lightSphereTransform2.position.x, 0.1f);
				ImGui::DragFloat3("Light Color", &lights[1].color.x, 0.1f, 0.0f, 1.0f);
			}
			//ImGui for setting light 3 position and color
			if (ImGui::CollapsingHeader("Light 3")) {
				ImGui::DragFloat3("Light Position", &lightSphereTransform3.position.x, 0.1f);
				ImGui::DragFloat3("Light Color", &lights[2].color.x, 0.1f, 0.0f, 1.0f);
			}
			//ImGui for num lights and bg color
			ImGui::DragInt("Number of Lights", &currentLights, 1.0f, 1, 3);
			ImGui::ColorEdit3("BG color", &bgColor.x);
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}


