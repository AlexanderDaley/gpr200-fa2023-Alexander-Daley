#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float gegeHeight;
uniform float gegeWidth;
uniform float time;
uniform float timeMod;

void main(){

	UV = vUV;

	mat3 rotation = mat3(
	cos(time*timeMod), sin(time*timeMod), 0.0,  //First column
	-sin(time*timeMod), cos(time*timeMod), 0.0,  //Second column
	0.0, 0.0, 0.0  //third column
	);

	vec3 newVPos = vec3(vPos.x*gegeHeight, vPos.y*gegeWidth, 0.0);

	gl_Position = vec4(newVPos*rotation,1.0);

}