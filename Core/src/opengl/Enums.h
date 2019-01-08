#pragma once

namespace core
{
	/* Types of buffer objects; used for VBOs and VAOs, etc */
	enum class BufferType
	{
		ARRAY_BUFFER			= 0x8892,	// Vertex attributes
		ELEMENT_ARRAY_BUFFER	= 0x8893,	// Vertex array indices
		UNIFORM_BUFFER			= 0x8A11,	// Uniform block storage
	};

	/* Types of buffer object uses; this is merely a hint for how the data will be used */
	enum class BufferUse
	{
		DYNAMIC_DRAW	= 0x88E8,	// Data will be modified repeatedly and used many times
		STATIC_DRAW		= 0x88E4,	// Data will be modified once and used many times
		STREAM_DRAW		= 0x88E0,	// Data will be modified once and used at most a few times
	};

	/* Types of color format used by OpenGL */
	enum class ColorFormat
	{
		RED		= 0x1903,
		GREEN	= 0x1904,
		BLUE	= 0x1905,
		ALPHA	= 0x1906,
		RGB		= 0x1907,
		RGBA	= 0x1908,
		BGR		= 0x80E0,
		BGRA	= 0x80E1,
	};

	/* Types of data format used by OpenGL */
	enum class DataFormat
	{
		BYTE			= 0x1400,
		UNSIGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		DOUBLE			= 0x140A,
	};

	/* Various filtering types for textures */
	enum class FilterType
	{
		NEAREST					= 0x2600,	// Selects the nearest texel
		LINEAR					= 0x2601,	// Selects a weighted average of the four nearest texels
		NEAREST_MIPMAP_NEAREST	= 0x2700,	// (Min only) Same as NEAREST, using the nearest mipmap
		LINEAR_MIPMAP_NEAREST	= 0x2701,	// (Min only) Same as LINEAR, using the nearest mipmap
		NEAREST_MIPMAP_LINEAR	= 0x2702,	// (Min only) Same as NEAREST, using the two nearest mipmaps
		LINEAR_MIPMAP_LINEAR	= 0x2703,	// (Min only) Same as LINEAR, using the two nearest mipmaps
	};

	/* Types of methods for rendering geometry */
	enum class RenderMode
	{
		POINTS			= 0x0000,
		LINES			= 0x0001,
		LINE_LOOP		= 0x0002,
		LINE_STRIP		= 0x0003,
		TRIANGLES		= 0x0004,
		TRIANGLE_STRIP	= 0x0005,
		TRIANGLE_FAN	= 0x0006,
		QUADS			= 0x0007,
		QUAD_STRIP		= 0x0008,
		POLYGON			= 0x0009,
	};

	/* Types of shaders */
	enum class ShaderType
	{
		FRAGMENT	= 0x8B30,
		GEOMETRY	= 0x8DD9,
		VERTEX		= 0x8B31,
	};

	/* Various types of textures */
	enum class TextureType
	{
		TEXTURE_1D			= 0x0DE0,
		TEXTURE_2D			= 0x0DE1,
		TEXTURE_2D_ARRAY	= 0x8C1A,
		TEXTURE_3D			= 0x806F,
	};

	/* Types of texture wrapping */
	enum class WrapType
	{
		REPEAT			= 0x2901,	// Ignores integer part of uv coordinate, forming a repeating pattern
		REPEAT_MIRROR	= 0x8370,	// Same as REPEAT but pattern is mirrored when integer part is odd
		CLAMP_TO_EDGE	= 0x812F,	// The uv coordinate is clamped to the range [0, 1]
		CLAMP_TO_BORDER	= 0x812D,	// The coordinates outside the range [0, 1] will be given a specific border color
	};
}