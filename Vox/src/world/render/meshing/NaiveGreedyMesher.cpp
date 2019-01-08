
#include "NaiveGreedyMesher.h"

#include "world/data/BlockRegion.h"
#include "world/render/BlockRenderRegistry.h"
#include "world/render/ChunkModel.h"
#include "world/util/Side.h"

void vox::Layer::set(const glm::uvec2 & pos, const LayerData & data)
{
	m_data[pos.y * CHUNK_SIZE<unsigned int> + pos.x] = data;
}
vox::LayerData & vox::Layer::get(const glm::uvec2 & pos)
{
	return m_data[pos.y * CHUNK_SIZE<unsigned int> + pos.x];
}
const vox::LayerData & vox::Layer::get(const glm::uvec2 & pos) const
{
	return m_data[pos.y * CHUNK_SIZE<unsigned int> + pos.x];
}

bool vox::Layer::extract(LayerData & data, glm::uvec2 & position, glm::uvec2 & size)
{
	// If the entire layer has been traversed, that is where it all ends
	if (!nextExtractionPosition(m_pos))
		return false;
	data = get(m_pos);
	position = m_pos;
	size = findExtractionSize();

	// Must make sure to remove the extracted data from the layer
	glm::uvec2 offset;
	for (offset.y = 0; offset.y < size.y; ++offset.y)
	for (offset.x = 0; offset.x < size.x; ++offset.x)
		get(m_pos + offset).clear();
	return true;
}

bool vox::Layer::nextExtractionPosition(glm::uvec2 & pos) const
{
	for (; pos.y < CHUNK_SIZE<unsigned int>; ++pos.y, pos.x = 0)
	for (; pos.x < CHUNK_SIZE<unsigned int>; ++pos.x)
	{
		if (!get(pos).empty())
			return true;
	}
	return false;
}
glm::uvec2 vox::Layer::findExtractionSize() const
{
	const auto data = get(m_pos);

	glm::uvec2 pos = m_pos;
	// Step along the x-axis until different data is encountered
	for (; pos.x < CHUNK_SIZE<unsigned int> && data == get(pos); ++pos.x)
		;
	// Step along the y-axis until different data is encountered
	for (; pos.y < CHUNK_SIZE<unsigned int>; ++pos.y)
	for (unsigned int k = m_pos.x; k < pos.x; ++k)
	{
		if (data != get({ k, pos.y }))
			return pos - m_pos;
	}
	return pos - m_pos;
}

// ...

void vox::NaiveGreedyMesher::mesh()
{
	for (const auto & side : Side::FACES)
		for (unsigned int i = 0; i < CHUNK_SIZE<unsigned int>; ++i)
			meshLayer(i, getLayer(i, side), side);
}
void vox::NaiveGreedyMesher::meshLayer(unsigned int depth, Layer && layer, const Side & side)
{
	LayerData data;
	glm::uvec2 pos, size;
	while (layer.extract(data, pos, size))
	{
		glm::vec3 p, s;
		p[side.dimension().x] = static_cast<float>(depth);
		p[side.dimension().y] = static_cast<float>(pos.x);
		p[side.dimension().z] = static_cast<float>(pos.y);
		s[side.dimension().x] = 1.0f;
		s[side.dimension().y] = static_cast<float>(size.x);
		s[side.dimension().z] = static_cast<float>(size.y);
		meshFace(data, p, s, side);
	}
}
void vox::NaiveGreedyMesher::meshFace(
	const LayerData & face,
	const glm::vec3 & pos,
	const glm::vec3 & size,
	const Side & side
)
{
	static const float shadows[7] = { 0.95f, 0.8f, 1.0f, 0.75f, 0.9f, 0.7f, 1.0f };

	const auto & block = m_blocks[face.block().getType()];
	const auto & indices = block.m_model.m_indices[side.id()];
	const auto & vertices = block.m_model.m_vertices[side.id()];
	if (indices.empty() || vertices.empty())
		return;

	const auto light = glm::vec4{ face.block().getLight() + 1ui8 } * (shadows[side.id()] / MAX_BLOCK_LIGHT);
	const auto scale = glm::vec3{ size[side.dimension().y], size[side.dimension().z], 1.0f};

	for (const auto & vertex : vertices)
		m_model.m_mesh.vertices().push_back(ChunkVertex{
			vertex.m_position * size + pos,
			vertex.m_normal,
			vertex.m_uv * scale,
			light,
			face.texture().getData()
		});
	for (const auto & index : indices)
		m_model.m_mesh.indices().push_back(index + m_totalVertices);
	m_totalVertices += vertices.size();
}

vox::Layer vox::NaiveGreedyMesher::getLayer(unsigned int depth, const Side & side) const
{
	Layer layer;

	const auto dim = side.dimension();

	glm::ivec3 pos;
	pos[dim.x] = depth;
	for (pos[dim.z] = 0; pos[dim.z] < CHUNK_SIZE<int>; ++pos[dim.z])
	for (pos[dim.y] = 0; pos[dim.y] < CHUNK_SIZE<int>; ++pos[dim.y])
		layer.set({ pos[dim.y], pos[dim.z] }, getData(pos, side));
	return layer;
}
vox::LayerData vox::NaiveGreedyMesher::getData(const glm::ivec3 & pos, const Side & side) const
{
	auto data = m_data.read(pos + side.normal());
	if (m_blocks[data.getType()].m_occlude[side.flip().id()])
		return LayerData{};

	const auto type = m_data.read(pos).getType();
	data.setType(type);
	return LayerData{ data, m_blocks[type].m_texture[side.id()].m_lookup(m_data, pos, side) };
}
