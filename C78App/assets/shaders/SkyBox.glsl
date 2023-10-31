// Simple SkyBox Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

layout(location = 0) out vec3 o_TexCoords;

layout(std140, binding = 0) uniform CameraUniformBuffer
{
	mat4 u_ProjectionMat;
	mat4 u_ViewMat;
	mat4 u_InvViewMat;
};

void main()
{
	vec4 pos = u_ProjectionMat * u_ViewMat * vec4(a_Position, 1.0);
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	o_TexCoords = vec3(pos.x, pos.y, pos.z);
}




#type fragment
#version 450 core

layout(location = 0) in vec3 o_TexCoords;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform samplerCube skybox;

void main()
{
	color = texture(skybox, o_TexCoords);
}