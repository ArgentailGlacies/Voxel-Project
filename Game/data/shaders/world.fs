#version 330 core

out vec4 color;

in Vertex
{
	vec3 normal;
	vec3 uv;
	vec4 color;
	flat uint texture;
} vertex;

uniform sampler2DArray textureArraySampler;

vec3 calculateUVCoordinate()
{
	uint offset = vertex.texture & 0x0000FFFFu;
	uint meta = (vertex.texture & 0x00FF0000u) >> 16u;
	uint hash = (vertex.texture & 0xFF000000u) >> 24u ^ (137u * uint(vertex.uv.x)) ^ (2467u * uint(vertex.uv.y));

	return vec3(vertex.uv.xy, vertex.uv.z + offset + hash % meta);
}
vec4 calculateTextureColor()
{
	return texture(textureArraySampler, calculateUVCoordinate());
}
vec3 calculateLightColor()
{
	vec3 sunlight = vec3(0.75, 0.75, 0.75);
	
	vec3 light = vertex.color.rgb + vertex.color.a * sunlight;
	light.r = clamp(light.r, 0.0, 1.0);
	light.g = clamp(light.g, 0.0, 1.0);
	light.b = clamp(light.b, 0.0, 1.0);
	return light;
}

void main()
{
	color = calculateTextureColor() * vec4(calculateLightColor(), 1);
}
