// Generic Texture/Color Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 5) in float a_TexIndex;
layout(location = 7) in int a_EntityID;

layout(std140, binding = 0) uniform CameraUniformBuffer
{
	mat4 u_ProjectionMat;
	mat4 u_ViewMat;
	mat4 u_InvViewMat;
};

layout(std140, binding = 1) uniform EntityUniformBuffer {
	mat4 u_TransformMat;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat float v_TexIndex;
layout (location = 3) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ProjectionMat * u_ViewMat * u_TransformMat * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 2) in flat float v_TexIndex;
layout (location = 3) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	o_Color = vec4(1.0, 1.0, 1.0, 1.0);
	o_EntityID = v_EntityID;
}
