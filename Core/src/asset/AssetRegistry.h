#pragma once

#include "asset/Asset.h"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace core
{
	template<typename Type> using Reference = typename Asset<Type>::Reference;
	template<typename Type> using Factory = typename Asset<Type>::Factory;

	class AssetRegistry
	{
	public:
		AssetRegistry() = default;
		AssetRegistry(const AssetRegistry &) = delete;
		AssetRegistry(AssetRegistry &&) = default;
		~AssetRegistry() = default;

		AssetRegistry & operator=(const AssetRegistry &) = delete;
		AssetRegistry & operator=(AssetRegistry &&) = delete;

		/**
			Checks whether there is an asset stored under the unique key or not.

			@return True iff there is an asset of the appropriate type under the given key.
		*/
		template<typename Type> bool has(const std::string & key) const;
		/**
			Adds a new asset and stores it under the given key. If another asset has been
			registered under the given key, then that asset will be replaced by the new asset.
			Assets of different types may use the same key without overwriting each other.

			@param key The unique key the asset should be stored under.
			@param factory The factory which creates the asset's resource.
		*/
		template<typename Type>
		void add(const std::string & key, const Factory<Type> & factory);
		/**
			Attempts to retrieve a reference to an asset under the given key. When referencing
			the asset, if it has not been referenced before, its resource will be created. When
			there are no more references to the asset, its resource will be released. The
			asset's resource is created by the provided factory.

			@param key The unique key to look up.
			@return A reference to the asset with the given key.
		*/
		template<typename Type>
		Reference<Type> get(const std::string & key) const;

	private:
		class AssetMapBase {};
		template<typename Type>
		class AssetMap : public AssetMapBase
		{
		public:
			bool has(const std::string & key) const;
			void add(const std::string & key, const Factory<Type> & factory);
			Reference<Type> get(const std::string & key) const;

		private:
			mutable std::unordered_map<std::string, Asset<Type>> m_assets;
		};

		std::unordered_map<std::type_index, std::unique_ptr<AssetMapBase>> m_maps;
	};

	// ...

	template<typename Type>
	inline bool AssetRegistry::has(const std::string & key) const
	{
		const auto it = m_maps.find(typeid(Type));
		if (it == m_maps.end())
			return false;
		return static_cast<AssetMap<Type>*>(it->second.get())->has(key);
	}

	template<typename Type>
	inline void AssetRegistry::add(const std::string & key, const Factory<Type> & factory)
	{
		const auto & type = typeid(Type);
		auto it = m_maps.find(type);
		if (it == m_maps.end())
			it = m_maps.emplace(type, std::make_unique<AssetMap<Type>>()).first;
		static_cast<AssetMap<Type>*>(it->second.get())->add(key, factory);
	}
	template<typename Type>
	inline Reference<Type> AssetRegistry::get(const std::string & key) const
	{
		const auto & type = typeid(Type);
		const auto it = m_maps.find(type);
		if (it == m_maps.end())
			throw std::invalid_argument(std::string{ "Type " } + type.name() + " was not found");
		return static_cast<AssetMap<Type>*>(it->second.get())->get(key);
	}

	// ...

	template<typename Type>
	inline bool AssetRegistry::AssetMap<Type>::has(const std::string & key) const
	{
		return m_assets.find(key) != m_assets.end();
	}

	template<typename Type>
	inline void AssetRegistry::AssetMap<Type>::add(
		const std::string & key,
		const Factory<Type> & factory
	) {
		m_assets.erase(key);
		m_assets.emplace(key, factory);
	}
	template<typename Type>
	inline Reference<Type> AssetRegistry::AssetMap<Type>::get(const std::string & key) const
	{
		const auto it = m_assets.find(key);
		if (it == m_assets.end())
			return {};
		return { key, it->second };
	}
}