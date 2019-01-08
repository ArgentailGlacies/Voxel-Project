#pragma once

#include "opengl/Attribute.h"
#include "opengl/Enums.h"
#include "opengl/VAO.h"
#include "opengl/VBO.h"

#include <vector>

namespace core
{
	class MeshBase
	{
	public:
		virtual ~MeshBase() = default;

		/**
			Stores the provided attribute in the mesh, enabling certain aspects of the mesh data to
			be accessible for use in shader programs. Each attribute of the mesh must be added for
			the mesh to render properly.

			@param attribute The attribute to add to the mesh.
		*/
		inline void addAttribute(const Attribute & attribute) { m_attributes.push_back(attribute); }

	protected:
		/**
			Initializes the mesh vbos by inserting the provided data into them. The data consists
			of an array of indices and an array of vertex data. Each entry in the indices data must
			point to a vertex which exists in the vertex array.

			@param inum The number of indices to copy over to the vbo.
			@param idata The index data to copy over to the vbo.
			@param vnum The number of vertices to copy over to the vbo.
			@param vdata The vertex data to copy over to the vbo.
			@param vsize The size of a single vertex, in bytes.
		*/
		bool build(
			unsigned int inum, const void * idata,
			unsigned int vnum, const void * vdata, unsigned int vsize
		);

		/**
			Renders up to the specified amount of indices stored in the mesh. If more indices than
			neccessary are specified, the excess will be discarded.

			@param mode The render mode the mesh should be rendered with.
			@param count The number of indices to render.
		*/
		void render(RenderMode mode, unsigned int count) const;

	private:
		VAO m_vao;
		VBO m_indexUBO{ BufferType::ELEMENT_ARRAY_BUFFER };
		VBO m_vertexUBO{ BufferType::ARRAY_BUFFER };

		std::vector<Attribute> m_attributes;
	};

	template<typename Vertex>
	class Mesh : public MeshBase
	{
	public:
		/**
			@return True iff there is no indices associated with the mesh.
		*/
		inline auto empty() const { return m_indexData.empty(); }

		/**
			@return The index data stored within the mesh.
		*/
		inline auto & indices() { return m_indexData; }
		/**
			@return The vertex data stored within the mesh.
		*/
		inline auto & vertices() { return m_vertexData; }

		/**
			Builds the mesh, preparing it for use within the rendering routines.
		*/
		bool build();
		/**
			Renders the mesh, do note this will not automatically unbind the underlying vbos.

			@mode The rendering method used to render the mesh.
		*/
		void render(RenderMode mode = RenderMode::TRIANGLES) const;

	private:
		std::vector<unsigned int> m_indexData;
		std::vector<Vertex> m_vertexData;
	};
}

namespace core
{
	template<typename Vertex>
	bool Mesh<Vertex>::build()
	{
		bool result = MeshBase::build(
			m_indexData.size(), m_indexData.data(),
			m_vertexData.size(), m_vertexData.data(), sizeof(Vertex)
		);

		std::vector<Vertex>{}.swap(m_vertexData);
		return result;
	}
	template<typename Vertex>
	inline void Mesh<Vertex>::render(RenderMode mode) const
	{
		MeshBase::render(mode, m_indexData.size());
	}
}