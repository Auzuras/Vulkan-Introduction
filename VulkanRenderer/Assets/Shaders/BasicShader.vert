#version 450

layout(binding = 0) uniform UniformModelData
{	
	mat4 model;
} ModelData;

layout(set = 1, binding = 0) uniform UniformCameraData
{
	mat4 view;
	mat4 projection;
} CameraData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTextCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTextCoord;

void main()
{
	gl_Position =  CameraData.projection * CameraData.view * ModelData.model * vec4(inPosition, 1.0);
    fragColor = inColor;
	fragTextCoord = inTextCoord;
}