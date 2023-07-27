#version 450


layout(location = 0) in vec3 in_fragColor;
layout(location = 1) in vec3 in_fragWorldPos;
layout(location = 2) in vec3 in_fragWorldNormal;
layout(location = 3) in vec2 in_texCoord;


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

layout(set = 2, binding = 0) uniform sampler2D material;

layout(push_constant) uniform Push{
	mat4 modelMat;
	mat4 normalMat;
} push;

void main(){
	vec3 diffuseLight = lightUBO.ambientLightColor.xyz * lightUBO.ambientLightColor.w;
	vec3 specLight = vec3(0.0);
	vec3 surfaceNormal = normalize(in_fragWorldNormal);

	vec3 camWorldPos = globalUBO.invViewMat[3].xyz;
	vec3 viewDirectiuon = normalize(camWorldPos - in_fragWorldPos);

	for(int i = 0; i < lightUBO.pointLightCount; i++){
		PointLight light = lightUBO.pointLights[i];

		vec3 directionToLight = light.position.xyz - in_fragWorldPos;
		float attenuation = 1.0 / dot(directionToLight,directionToLight);
		directionToLight = normalize(directionToLight);

		float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = light.color.xyz * light.color.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;

		vec3 specHalfAng = normalize(directionToLight + viewDirectiuon);
		float blinnTerm = dot(surfaceNormal, specHalfAng);
		blinnTerm = clamp(blinnTerm, 0, 1);
		blinnTerm = pow(blinnTerm, 512.0);
		specLight += intensity * blinnTerm;
	}
	out_Color = vec4((diffuseLight + specLight) * in_fragColor * texture(material, in_texCoord).rgb, 1.0);

}