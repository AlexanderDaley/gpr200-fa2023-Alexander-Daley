#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D wallTexture;
uniform sampler2D exTexture;
uniform sampler2D noiseTexture;

void main(){
	
	float noise = texture(noiseTexture,UV).r;
	vec2 uv = UV + noise * 0.1f;
	vec4 colorA = texture(wallTexture,uv);
	vec4 colorB = texture(exTexture,uv);
	vec3 color = mix(colorA.rgb,colorB.rgb,colorB.a * 0.5);

	FragColor = vec4(color, 1.0);

}
