#version 330 core

#include "data/shaders/common/matrices.vs"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inUV;
layout(location = 3) in vec4 inColor;
layout(location = 4) in uint inTexture;

out Vertex
{
	vec3 normal;
	vec3 uv;
	vec4 color;
	flat uint texture;
} vertex;

void main()
{
	gl_Position = matrices.projection * matrices.view * matrices.model * vec4(inPosition, 1.0);
	
	vertex.normal = inNormal;
	vertex.uv = inUV; 
	vertex.color = inColor;
	vertex.texture = inTexture;
}