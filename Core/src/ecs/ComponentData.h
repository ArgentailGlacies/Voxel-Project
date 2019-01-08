#pragma once

#include "ecs/Entity.h"

#include <algorithm>
#include <vector>

namespace core
{
	class ComponentDataBase {};

	template<typename Component>
	class ComponentData : public ComponentDataBase
	{
	public:
		ComponentData() = default;
		ComponentData(const ComponentData &) = delete;
		ComponentData(ComponentData &&) = delete;
		~ComponentData() = default;

		ComponentData & operator=(const ComponentData &) = delete;
		ComponentData & operator=(ComponentData &&) = delete;

		/**
			Retrieves the data stored under the given entity. If the data does not	exist for the
			given entity, the data is created.

			@param entity The entity to retrieve data for.
			@return The data associated with the given entity.
		*/
		Component & operator[](Entity entity);

		/**
			Checks if there is any data associated with the given entity.

			@param entity The entity to look under.
			@return True iff there is any data associated with the given entity.
		*/
		bool has(Entity entity) const;

	private:
		using Data = std::pair<Entity, Component>;

		std::vector<Data> m_data;
	};
}

// ...

namespace core
{
	template<typename Component>
	inline Component & ComponentData<Component>::operator[](Entity entity)
	{
		auto it = std::lower_bound(begin(m_data), end(m_data), entity,
			[](const auto & lhs, const auto & rhs) { return lhs.first < rhs; }
		);
		if (it == m_data.end() || it->first != entity)
			it = m_data.insert(it, std::make_pair(entity, Component{}));
		return it->second;
	}

	template<typename Component>
	inline bool ComponentData<Component>::has(Entity entity) const
	{
		const auto it = std::lower_bound(begin(m_data), end(m_data), entity,
			[](const auto & lhs, const auto & rhs) { return lhs.first < rhs; }
		); ;
		return it != m_data.end() && it->first == entity;
	}
}