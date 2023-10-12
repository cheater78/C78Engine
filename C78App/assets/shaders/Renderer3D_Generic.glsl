// Generic Texture/Color Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in float a_TexIndex;

layout(std140, binding = 0) uniform CameraUniformBuffer
{
	mat4 u_ProjectionMat;
	mat4 u_ViewMat;
	mat4 u_InvViewMat;
};

layout(std140, binding = 3) uniform EntityUniformBuffer {
	mat4 u_TransformMat;
	mat4 u_NormalMat;
};

layout (location = 0) out vec4 v_Color;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out flat float v_TexIndex;
layout (location = 3) out vec3 v_WorldPos;
layout (location = 4) out vec3 v_WorldNormal;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_WorldPos = (u_TransformMat * vec4(a_Position, 1.0)).xyz;
	v_WorldNormal = normalize( mat3(u_NormalMat) * a_Normal );

	v_TexIndex = a_TexIndex;

	gl_Position = u_ProjectionMat * u_ViewMat * u_TransformMat * vec4(a_Position, 1.0);
}









#type fragment
#version 450 core

#define PI 3.1415926535897932384626433832795

layout (location = 0) in vec4 v_Color;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in flat float v_TexIndex;
layout (location = 3) in vec3 v_WorldPos;
layout (location = 4) in vec3 v_WorldNormal;

layout (binding = 0) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 o_Color;

layout(std140, binding = 0) uniform CameraUniformBuffer{
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

layout(std140, binding = 2) uniform EntityMaterialUniformBuffer {
	float u_Defused;
	float u_Specular;
	float u_Opacity;
};

layout(std140, binding = 3) uniform EntityUniformBuffer {
	mat4 u_TransformMat;
	mat4 u_NormalMat;
};


vec4 textureColor(int textureIndex, vec4 tintColor, vec2 textureCoordinate) {
	switch(textureIndex) {
		case  0: tintColor *= texture(u_Textures[ 0], textureCoordinate); break;
		case  1: tintColor *= texture(u_Textures[ 1], textureCoordinate); break;
		case  2: tintColor *= texture(u_Textures[ 2], textureCoordinate); break;
		case  3: tintColor *= texture(u_Textures[ 3], textureCoordinate); break;
		case  4: tintColor *= texture(u_Textures[ 4], textureCoordinate); break;
		case  5: tintColor *= texture(u_Textures[ 5], textureCoordinate); break;
		case  6: tintColor *= texture(u_Textures[ 6], textureCoordinate); break;
		case  7: tintColor *= texture(u_Textures[ 7], textureCoordinate); break;
		case  8: tintColor *= texture(u_Textures[ 8], textureCoordinate); break;
		case  9: tintColor *= texture(u_Textures[ 9], textureCoordinate); break;
		case 10: tintColor *= texture(u_Textures[10], textureCoordinate); break;
		case 11: tintColor *= texture(u_Textures[11], textureCoordinate); break;
		case 12: tintColor *= texture(u_Textures[12], textureCoordinate); break;
		case 13: tintColor *= texture(u_Textures[13], textureCoordinate); break;
		case 14: tintColor *= texture(u_Textures[14], textureCoordinate); break;
		case 15: tintColor *= texture(u_Textures[15], textureCoordinate); break;
		case 16: tintColor *= texture(u_Textures[16], textureCoordinate); break;
		case 17: tintColor *= texture(u_Textures[17], textureCoordinate); break;
		case 18: tintColor *= texture(u_Textures[18], textureCoordinate); break;
		case 19: tintColor *= texture(u_Textures[19], textureCoordinate); break;
		case 20: tintColor *= texture(u_Textures[20], textureCoordinate); break;
		case 21: tintColor *= texture(u_Textures[21], textureCoordinate); break;
		case 22: tintColor *= texture(u_Textures[22], textureCoordinate); break;
		case 23: tintColor *= texture(u_Textures[23], textureCoordinate); break;
		case 24: tintColor *= texture(u_Textures[24], textureCoordinate); break;
		case 25: tintColor *= texture(u_Textures[25], textureCoordinate); break;
		case 26: tintColor *= texture(u_Textures[26], textureCoordinate); break;
		case 27: tintColor *= texture(u_Textures[27], textureCoordinate); break;
		case 28: tintColor *= texture(u_Textures[28], textureCoordinate); break;
		case 29: tintColor *= texture(u_Textures[29], textureCoordinate); break;
		case 30: tintColor *= texture(u_Textures[30], textureCoordinate); break;
		case 31: tintColor *= texture(u_Textures[31], textureCoordinate); break;
	}
	return tintColor;
}

struct FragmentLight {
	vec3 ambientLight;
	vec3 specularLight;
};

FragmentLight calculateFragmentLighting(vec4 lightColor, vec3 fragmentDirectionToLight, vec3 fragmentSurfaceNormal, vec3 viewDirection) {
	FragmentLight fragmentLighting;
	
	vec3 attenuatedLightColor = lightColor.xyz * lightColor.w * (1.0 / dot(fragmentDirectionToLight, fragmentDirectionToLight));
	
	fragmentLighting.ambientLight = attenuatedLightColor * max(dot(fragmentSurfaceNormal, normalize(fragmentDirectionToLight)), 0);
	fragmentLighting.specularLight = attenuatedLightColor * pow(clamp(dot(fragmentSurfaceNormal, normalize(fragmentDirectionToLight + viewDirection)), 0, 1), 512.0);
	return fragmentLighting;
}

void main() {
	vec4 fragColor = textureColor(int(v_TexIndex), v_Color, v_TexCoord);
	if (fragColor.a == 0.0) discard;

	vec3 diffuseLight = u_AmbientLight.color.xyz * u_AmbientLight.color.w;
	vec3 specLight = vec3(0.0);
	vec3 surfaceNormal = normalize(v_WorldNormal);
	vec3 viewDirection = normalize(u_InvViewMat[3].xyz - v_WorldPos);

	if(dot(viewDirection, surfaceNormal) < 0) discard;													//BackFace Culling

	for(int i = 0; i < u_DirectLightCount; i++){
		DirectLight light = u_DirectLights[i];
		diffuseLight += light.color.xyz * light.color.w * max(dot(surfaceNormal, -light.direction), 0);
	}

	for(int i = 0; i < u_PointLightCount; i++){
		PointLight light = u_PointLights[i];
		vec3 directionToLight = light.position - v_WorldPos;

		FragmentLight fragmentLight = calculateFragmentLighting(light.color, directionToLight , surfaceNormal, viewDirection);
		diffuseLight += fragmentLight.ambientLight;
		specLight += fragmentLight.specularLight;
	}

	for(int i = 0; i < u_SpotLightCount; i++){
		SpotLight light = u_SpotLights[i];
		vec3 directionToLight = light.position - v_WorldPos;

		float edgeCutoff = clamp(1-((acos(dot(normalize(directionToLight), normalize(-light.direction)))-light.angle) / light.edgeAngle), 0, 1);
		if(edgeCutoff == 0.0) continue;

		FragmentLight fragmentLight = calculateFragmentLighting(light.color, directionToLight , surfaceNormal, viewDirection);
		diffuseLight += fragmentLight.ambientLight * edgeCutoff;
		specLight += fragmentLight.specularLight * edgeCutoff;
	}

	o_Color = vec4((diffuseLight*u_Defused + specLight*u_Specular) * fragColor.xyz, u_Opacity * fragColor.w);
}
