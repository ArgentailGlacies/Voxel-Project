#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>

namespace vox
{
	class Side
	{
	public:
		static const Side POS_X;
		static const Side POS_Y;
		static const Side POS_Z;
		static const Side NEG_X;
		static const Side NEG_Y;
		static const Side NEG_Z;
		static const Side OTHER;

		static const Side AXIS[3];
		static const Side SIDES[4];
		static const Side FACES[6];
		static const Side ALL[7];

		static const Side & fromName(const std::string & name);
		static std::vector<Side> fromList(const std::string & list);

		// ...

		inline bool operator==(const Side & other) const { return m_id == other.m_id; }
		inline bool operator!=(const Side & other) const { return m_id != other.m_id; }

		inline auto & name() const { return m_name; }
		inline auto id() const { return m_id; }
		inline auto & flip() const { return m_opposite; }

		inline auto positive() const { return m_id < 3; }

		inline auto & normal() const { return m_normal; }
		inline auto & tangentH() const { return m_tangentH; }
		inline auto & tangentV() const { return m_tangentV; }

		inline auto & dimension() const { return m_dimension; }

	private:
		Side(const std::string & name, unsigned int id, const Side & opposite,
			const glm::ivec3 & normal, const glm::ivec3 & tangentH, const glm::ivec3 & tangentV,
			const glm::ivec3 & dimension
		) :
			m_name(name),
			m_id(id),
			m_opposite(opposite),
			m_normal(normal),
			m_tangentH(tangentH),
			m_tangentV(tangentV),
			m_dimension(dimension)
		{}

		const std::string m_name;
		const unsigned int m_id;
		const Side & m_opposite;

		const glm::ivec3 m_normal;
		const glm::ivec3 m_tangentH;
		const glm::ivec3 m_tangentV;

		const glm::uvec3 m_dimension;
	};
}