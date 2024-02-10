#type compute
#version 460 core

float PI = 3.1415926535;

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D mainTex;

layout(std140, binding = 1) uniform CameraUniformBuffer
{
	mat4 u_ProjectionMat;
	mat4 u_ViewMat;
	mat4 u_InvViewMat;
};

void main()
{
	vec4 color = vec4(0.075, 0.133, 0.173, 1.0);

	ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
	
	ivec2 dims = imageSize(mainTex);
	vec2 uv = vec2( -(float((pixelCoords.x + 0.5) * 2 - dims.x) / dims.x), -(float((pixelCoords.y + 0.5) * 2 - dims.y) / dims.y));

	float fov = 2.0*atan( 1.0/u_ProjectionMat[1][1] ) * 180.0 / PI;
	float aspect = dims.x / dims.y;

	vec3 cam_r = vec3(u_InvViewMat[0][0], u_InvViewMat[0][1], u_InvViewMat[0][2]);
	vec3 cam_u = vec3(u_InvViewMat[1][0], u_InvViewMat[1][1], u_InvViewMat[1][2]);
	vec3 cam_d = vec3(u_InvViewMat[2][0], u_InvViewMat[2][1], u_InvViewMat[2][2]);
	vec3 cam_o = vec3(u_InvViewMat[3][0], u_InvViewMat[3][1], u_InvViewMat[3][2]);

	vec3 ray_o = cam_o;
	vec3 ray_d = vec3( vec4(uv.x, uv.y, 1.0, 0.0) * u_InvViewMat );
	ray_d = normalize(ray_d);


	vec3 sphere_c = vec3(0.0, 0.0, -5.0);
	float sphere_r = 1.0;

	vec3 o_c = ray_o - sphere_c;
	float b = dot(ray_d, o_c);
	float c = dot(o_c, o_c) - sphere_r * sphere_r;
	float intersectionState = b * b - c;
	vec3 intersection = ray_o + ray_d * (-b + sqrt(b * b - c));

	if (intersectionState >= 0.0)
	{
		color = vec4((normalize(intersection - sphere_c) + 1.0) / 2.0, 1.0);
	}

	imageStore(mainTex, pixelCoords, color);
}