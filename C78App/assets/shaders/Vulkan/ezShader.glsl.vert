#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in float in_texID;
layout(location = 4) in vec2 in_uv;

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out vec3 out_fragWorldPos;
layout(location = 2) out vec3 out_fragWorldNormal;


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

layout(push_constant) uniform Push{
	mat4 modelMat;
	mat4 normalMat;
} push;


void main() {
	mat4 projViewMat = globalUBO.projMat * globalUBO.viewMat;

	vec4 posWorld = push.modelMat * vec4(in_position, 1.0);
	gl_Position = projViewMat * push.modelMat * vec4(in_position, 1.0);

	out_fragWorldNormal = normalize(mat3(push.normalMat) * in_normal);
	out_fragWorldPos = posWorld.xyz;
	out_fragColor = in_color;
}