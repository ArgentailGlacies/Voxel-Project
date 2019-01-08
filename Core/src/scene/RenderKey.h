#pragma once

#include "scene/Enums.h"

namespace core
{
	class RenderKey
	{
	public:
		using Data = unsigned long long;

		inline void setViewportLayer(ViewportLayer layer) { m_viewportLayer = layer; }
		inline void setTranslucency(Translucency translucency) { m_translucency = translucency; }
		inline void setViewport(unsigned int viewport) { m_viewport = viewport; }
		inline void setProgram(unsigned int program) { m_program = program; }
		inline void setTexture(unsigned int texture) { m_texture = texture; }
		inline void setDepth(unsigned int depth) { m_depth = depth; }

		inline auto getViewportLayer() const { return m_viewportLayer; }
		inline auto getTranslucency() const { return m_translucency; }
		inline auto getViewport() const { return m_viewport; }
		inline auto getProgram() const { return m_program; }
		inline auto getTexture() const { return m_texture; }
		inline auto getDepth() const { return m_depth; }

		/**
			Converts the key into a rendering priority such that the render queue may properly order
			all tasks in the correct order. Rendering tasks are ordered by texture and program, such
			that there will be as few state changes as possible.
			
			In general the geometry is sorted by material first, then distance (front to back), but
			if the task requires transparency, the geometry is first sorted by distance (back to
			front), then by material.
		*/
		Data compile() const;

	private:
		ViewportLayer m_viewportLayer = ViewportLayer::WORLD;
		Translucency m_translucency = Translucency::OPAQUE;
		unsigned int m_viewport = 0;
		unsigned int m_program = 0;
		unsigned int m_texture = 0;
		unsigned int m_depth = 0;
	};
}