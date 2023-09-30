#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D gegeTexture;

void main(){
	

	FragColor = texture (gegeTexture, UV);

}
