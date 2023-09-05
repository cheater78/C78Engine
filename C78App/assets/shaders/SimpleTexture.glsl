// Texture 3D Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;

layout(binding = 0) uniform CameraData{
	mat4 u_ViewProjection;
};

layout(binding = 1) uniform TransformData{
	mat4 u_Transform;
};


struct VertexOutput
{
	vec2 texCoords;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.texCoords = a_TexCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;

struct VertexOutput
{
	vec2 texCoords;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D u_Textures[32];




void main()
{
	color = texture(u_Textures[ 0], Input.texCoords); //vec4(Input.texCoords, 0.0, 1.0); //
}
