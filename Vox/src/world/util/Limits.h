#pragma once

namespace vox
{
	constexpr unsigned int MAX_BLOCK_TYPE = 4'095;
	constexpr unsigned int MAX_BLOCK_LIGHT = 31;
	constexpr unsigned int MAX_BLOCK_TEXTURE_ID = 65'535;
	constexpr unsigned int MAX_BLOCK_TEXTURE_META = 255;
	constexpr unsigned int MAX_BLOCK_TEXTURE_HASH = 255;

	constexpr unsigned int MAX_WORLD_QUERY = 67'108'864;

	// ...

	template<typename T> constexpr T CHUNK_LG = T(5);
	template<typename T> constexpr T CHUNK_SIZE = T(T(1) << CHUNK_LG<T>);
	template<typename T> constexpr T CHUNK_BIT = T(CHUNK_SIZE<T> - T(1));
	template<typename T> constexpr T CHUNK_AREA = T(CHUNK_SIZE<T> * CHUNK_SIZE<T>);
	template<typename T> constexpr T CHUNK_VOLUME = T(CHUNK_AREA<T> * CHUNK_SIZE<T>);

	template<typename T> constexpr T REGION_LG = T(4);
	template<typename T> constexpr T REGION_SIZE = T(T(1) << REGION_LG<T>);
	template<typename T> constexpr T REGION_BIT = T(REGION_SIZE<T> - T(1));
	template<typename T> constexpr T REGION_AREA = T(REGION_SIZE<T> * REGION_SIZE<T>);
	template<typename T> constexpr T REGION_VOLUME = T(REGION_AREA<T> * REGION_SIZE<T>);
}