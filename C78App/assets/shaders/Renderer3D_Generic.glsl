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

layout(std140, binding = 1) uniform SceneLightUniform{
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

void main()
{
	vec4 fragColor = v_Color;

	switch(int(v_TexIndex))
	{
		case  0: fragColor *= texture(u_Textures[ 0], v_TexCoord); break;
		case  1: fragColor *= texture(u_Textures[ 1], v_TexCoord); break;
		case  2: fragColor *= texture(u_Textures[ 2], v_TexCoord); break;
		case  3: fragColor *= texture(u_Textures[ 3], v_TexCoord); break;
		case  4: fragColor *= texture(u_Textures[ 4], v_TexCoord); break;
		case  5: fragColor *= texture(u_Textures[ 5], v_TexCoord); break;
		case  6: fragColor *= texture(u_Textures[ 6], v_TexCoord); break;
		case  7: fragColor *= texture(u_Textures[ 7], v_TexCoord); break;
		case  8: fragColor *= texture(u_Textures[ 8], v_TexCoord); break;
		case  9: fragColor *= texture(u_Textures[ 9], v_TexCoord); break;
		case 10: fragColor *= texture(u_Textures[10], v_TexCoord); break;
		case 11: fragColor *= texture(u_Textures[11], v_TexCoord); break;
		case 12: fragColor *= texture(u_Textures[12], v_TexCoord); break;
		case 13: fragColor *= texture(u_Textures[13], v_TexCoord); break;
		case 14: fragColor *= texture(u_Textures[14], v_TexCoord); break;
		case 15: fragColor *= texture(u_Textures[15], v_TexCoord); break;
		case 16: fragColor *= texture(u_Textures[16], v_TexCoord); break;
		case 17: fragColor *= texture(u_Textures[17], v_TexCoord); break;
		case 18: fragColor *= texture(u_Textures[18], v_TexCoord); break;
		case 19: fragColor *= texture(u_Textures[19], v_TexCoord); break;
		case 20: fragColor *= texture(u_Textures[20], v_TexCoord); break;
		case 21: fragColor *= texture(u_Textures[21], v_TexCoord); break;
		case 22: fragColor *= texture(u_Textures[22], v_TexCoord); break;
		case 23: fragColor *= texture(u_Textures[23], v_TexCoord); break;
		case 24: fragColor *= texture(u_Textures[24], v_TexCoord); break;
		case 25: fragColor *= texture(u_Textures[25], v_TexCoord); break;
		case 26: fragColor *= texture(u_Textures[26], v_TexCoord); break;
		case 27: fragColor *= texture(u_Textures[27], v_TexCoord); break;
		case 28: fragColor *= texture(u_Textures[28], v_TexCoord); break;
		case 29: fragColor *= texture(u_Textures[29], v_TexCoord); break;
		case 30: fragColor *= texture(u_Textures[30], v_TexCoord); break;
		case 31: fragColor *= texture(u_Textures[31], v_TexCoord); break;
	}

	if (fragColor.a == 0.0)
		discard;
	
	vec3 diffuseLight = u_AmbientLight.color.xyz * u_AmbientLight.color.w;
	vec3 specLight = vec3(0.0);
	vec3 surfaceNormal = normalize(v_WorldNormal);

	vec3 camWorldPos = u_InvViewMat[3].xyz;
	vec3 viewDirection = normalize(camWorldPos - v_WorldPos);

	for(int i = 0; i < u_DirectLightCount; i++){
		DirectLight light = u_DirectLights[i];

		float cosAngIncidence = max(dot(surfaceNormal, -light.direction), 0);
		vec3 intensity = light.color.xyz * light.color.w;

		diffuseLight += intensity * cosAngIncidence;
	}

	for(int i = 0; i < u_PointLightCount; i++){
		PointLight light = u_PointLights[i];

		vec3 directionToLight = light.position - v_WorldPos;
		float attenuation = 1.0 / dot(directionToLight, directionToLight);
		directionToLight = normalize(directionToLight);

		float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
		vec3 intensity = light.color.xyz * light.color.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;

		vec3 specHalfAng = normalize(directionToLight + viewDirection);
		float blinnTerm = dot(surfaceNormal, specHalfAng);
		blinnTerm = clamp(blinnTerm, 0, 1);
		blinnTerm = pow(blinnTerm, 512.0);
		specLight += intensity * blinnTerm;
	}

	for(int i = 0; i < u_SpotLightCount; i++){
		SpotLight light = u_SpotLights[i];

		vec3 directionToLight = light.position - v_WorldPos;
		float attenuation = 1.0 / dot(directionToLight, directionToLight);
		directionToLight = normalize(directionToLight);

		float edgeCutoff = clamp(1-((acos(dot(normalize(directionToLight), normalize(-light.direction)))-light.angle) / light.edgeAngle), 0, 1);

		float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
		vec3 intensity = light.color.xyz * light.color.w * attenuation;

		diffuseLight += intensity * cosAngIncidence * edgeCutoff;

		vec3 specHalfAng = normalize(directionToLight + viewDirection);
		float blinnTerm = dot(surfaceNormal, specHalfAng);
		blinnTerm = clamp(blinnTerm, 0, 1);
		blinnTerm = pow(blinnTerm, 512.0);
		specLight += intensity * blinnTerm * edgeCutoff;
	}

	o_Color = vec4((diffuseLight*u_Defused + specLight*u_Specular) * fragColor.xyz, u_Opacity); // vec4(); // vec4((diffuseLight + specLight), 1.0) * fragColor;
	
	//o_Color = vec4((surfaceNormal.x+1)/2, (surfaceNormal.y+1)/2, (surfaceNormal.z+1)/2, 1.0);
}
