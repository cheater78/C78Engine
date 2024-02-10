// Simple Light Shader to Render a Circle Sprite

#type vertex
#version 450 core

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2( 1.0, -1.0),
  vec2( 1.0,  1.0),
  vec2( 1.0,  1.0),
  vec2(-1.0,  1.0),
  vec2(-1.0, -1.0)
);

layout(std140, binding = 0) uniform CameraUniformBuffer
{
	mat4 u_ProjectionMat;
	mat4 u_ViewMat;
	mat4 u_InvViewMat;
};


struct AmbientLight {
	vec4 color;
};

struct DirectLight {
	vec3 direction;
	vec4 color;
};

struct PointLight {
	vec3 position;
	vec4 color;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec4 color;
	float angle;
	float edgeAngle;
};

layout(std140, binding = 1) uniform SceneLightUniform {
	AmbientLight u_AmbientLight;

	DirectLight u_DirectLights[10];
	int u_DirectLightCount;
	PointLight u_PointLights[10];
	int u_PointLightCount;
	SpotLight u_SpotLights[10];
	int u_SpotLightCount;
};


layout (location = 0) out vec4 v_Color;
layout (location = 1) out vec2 v_BaseQuadPos;

void main()
{
	int indexID = int(gl_VertexIndex / 6);
	int vertexID = gl_VertexIndex % 6;
	vec3 worldCamRight = {u_ViewMat[0][0], u_ViewMat[1][0], u_ViewMat[2][0]};
	vec3 worldCamUp = {u_ViewMat[0][1], u_ViewMat[1][1], u_ViewMat[2][1]};
	
	vec2 baseQuad = OFFSETS[vertexID];

	vec3 worldPos = u_PointLights[indexID].position
		+ clamp(0.1 * u_PointLights[indexID].color.w, 0.0, 1.0) * baseQuad.x * normalize(worldCamRight)
		+ clamp(0.1 * u_PointLights[indexID].color.w, 0.0, 1.0) * baseQuad.y * normalize(worldCamUp);
	
	v_Color = u_PointLights[indexID].color;
	v_BaseQuadPos = baseQuad;
	gl_Position = u_ProjectionMat * u_ViewMat * vec4(worldPos, 1.0);
}




#type fragment
#version 450 core

#define PI 3.1415926535897932384626433832795

layout (location = 0) in vec4 v_Color;
layout (location = 1) in vec2 v_BaseQuadPos;

layout(location = 0) out vec4 o_Color;

layout(std140, binding = 0) uniform CameraUniformBuffer{
	mat4 u_ProjectionMat;
	mat4 u_ViewMat;
	mat4 u_InvViewMat;
};

void main() {
	float distance = sqrt(dot(v_BaseQuadPos, v_BaseQuadPos));

	if(distance >= 1.0) discard;

	o_Color = vec4(v_Color.xzy, 0.7 * max(v_Color.w, 1.0) * 0.5*(cos(distance*PI) + 1.0) );
}
