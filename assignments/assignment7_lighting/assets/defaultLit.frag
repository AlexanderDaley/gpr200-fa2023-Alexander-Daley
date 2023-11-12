#version 450
out vec4 FragColor;

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

struct Material {
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
};

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 3
uniform Light _Lights[MAX_LIGHTS];
uniform Material _Material;
uniform vec3 camPos;
uniform int currentLights;
uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec4 texshure = texture(_Texture,fs_in.UV);
	vec3 meshColor = vec3 (0.0f,0.0f,0.0f);
	vec3 v = normalize(camPos - fs_in.WorldPosition);

	for(int i = 0; i < currentLights; i++){
	vec3 w = normalize(_Lights[i].position-fs_in.WorldPosition);
	vec3 h = normalize((w + v) / length(w + v));
		meshColor += _Lights[i].color*_Material.ambientK;
		meshColor += _Lights[i].color*(_Material.diffuseK*max(dot(normal,w),0));
		meshColor += _Lights[i].color*(_Material.specular*pow(max(dot(h,normal),0),_Material.shininess));
	}

	FragColor = vec4(texshure.rgb * meshColor, 1.0f);
}