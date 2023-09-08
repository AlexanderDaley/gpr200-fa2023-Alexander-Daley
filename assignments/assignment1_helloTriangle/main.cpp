#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

//global variable in main.cpp or in main()
float vertices[21] = {
	//x  //y  //z   //r   //g  //b   //a
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, //bottom left
	0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,  //bottom right
	0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0   //top center
};

//Vertex shader
const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	layout(location = 1) in vec4 vColor;
	out vec4 Color;
	void main(){
		Color = vColor;
		gl_Position = vec4(vPos,1.0);
	}
)";

//Fragment shader - colors the pixels
const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	in vec4 Color;
	void main(){
		FragColor = Color;
	}
)";

//Creates a new vertex array object with vertex data
unsigned int createVAO(float* vertexData, int numVertices, int totalLength) {

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//allocate space for + send vertex data to gpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalLength, vertexData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * totalLength/numVertices, (const void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * totalLength/numVertices, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	return(vao);

};

unsigned int createShader(GLenum shaderType, const char* sourceCode) {
	//Create a new vertex shader object
	unsigned int shader = glCreateShader(shaderType);
	//Supply the shader object with source code
	glShaderSource(shader, 1, &sourceCode, NULL);
	//Compile the shader object
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}
	return(shader);
};

//Creates a new shader program with vertex + fragment stages
//Returns id of new shader program if successful, 0 if failed
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	//Create a new vertex shader object
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	//Create a new fragment shader object
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);

	//glLinkProgram...
	//Check for linking errors
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}
	//The linked program now contains our compiled code, so we can delete these intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return(shaderProgram);
}


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	unsigned int vao = createVAO(vertices, 3, 21);
	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

	}
	printf("Shutting down...");
}
