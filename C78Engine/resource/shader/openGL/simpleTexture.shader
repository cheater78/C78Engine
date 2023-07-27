#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texID;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_texID;

uniform mat4 u_MVP;

void main()
{
	
	gl_Position = u_MVP * position;
	
	v_Color = color;
	v_TexCoord = texCoord;
	v_texID = texID;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_texID;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};