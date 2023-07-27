#version 450

layout(location = 0) in vec2 in_fragOffset;
layout(location = 1) in vec4 lightColor;

layout(location = 0) out vec4 out_Color;

struct DirectLight {
	vec4 direction;
	vec4 color;
	float intensity;
};

struct PointLight{
	vec4 position;
	vec4 color;
	float radius;
};

layout(set = 0, binding = 0) uniform GlobalUbo{
	mat4 projMat;
	mat4 viewMat;
	mat4 invViewMat;
} globalUBO;

layout(set = 1, binding = 0) uniform LightUbo{
	vec4 ambientLightColor;
	DirectLight directLights[10];
	int directLightCount;
	PointLight pointLights[10];
	int pointLightCount;
} lightUBO;

const float M_PI = 3.14159265358;

void main(){
	float fragDist = sqrt(dot(in_fragOffset, in_fragOffset));

	if(fragDist >= 1.0){
		discard;
	}

	out_Color = lightColor;
	out_Color.w = 0.5*(cos(fragDist*M_PI) + 1.0);
}