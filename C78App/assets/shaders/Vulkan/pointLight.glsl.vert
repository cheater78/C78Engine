#version 450

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout(location = 0) out vec2 out_fragOffset;
layout(location = 1) out vec4 lightColor;

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

void main(){
	int lightID = int(gl_VertexIndex / 6);
	int vertID = gl_VertexIndex % 6;

	out_fragOffset = OFFSETS[vertID];

	vec3 worldCamRight = {globalUBO.viewMat[0][0], globalUBO.viewMat[1][0], globalUBO.viewMat[2][0]};
	vec3 worldCamUp = {globalUBO.viewMat[0][1], globalUBO.viewMat[1][1], globalUBO.viewMat[2][1]};

	vec3 worldPos = lightUBO.pointLights[lightID].position.xyz
		+ lightUBO.pointLights[lightID].radius * out_fragOffset.x * worldCamRight
		+ lightUBO.pointLights[lightID].radius * out_fragOffset.y * worldCamUp;

	lightColor = lightUBO.pointLights[lightID].color;
	gl_Position = globalUBO.projMat * globalUBO.viewMat * vec4(worldPos, 1.0);
}