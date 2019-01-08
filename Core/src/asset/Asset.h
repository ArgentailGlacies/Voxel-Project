#pragma once

#include <functional>
#include <memory>
#include <string>

namespace core
{
	template<typename Type>
	class Asset
	{
	public:
		class Reference
		{
		public:
			Reference() = default;
			Reference(const std::string & name, Asset & asset);
			Reference(const Reference & other);
			Reference(Reference && other);
			~Reference();

			Reference & operator=(const Reference & other);
			Reference & operator=(Reference && other);

			bool operator==(const Reference & other) const;
			bool operator!=(const Reference & other) const;

			inline auto name() const { return m_name; }

			inline operator Type&() const { return *m_asset->m_resource; }
			inline Type * operator->() const { return m_asset->m_resource.get(); }

			inline bool empty() const { return m_asset == nullptr; }

		private:
			void increment() const { if (m_asset != nullptr) m_asset->increment(); }
			void decrement() const { if (m_asset != nullptr) m_asset->decrement(); }

			std::string m_name;

			Asset * m_asset = nullptr;
		};

		// ...

		using TypePtr = std::unique_ptr<Type>;
		using Factory = std::function<TypePtr()>;

		Asset();
		Asset(const Factory & factory);
		Asset(const Asset &) = default;
		Asset(Asset &&) = default;
		~Asset() = default;

		Asset & operator=(const Asset &) = default;
		Asset & operator=(Asset &&) = default;

		inline bool empty() const { return m_resource == nullptr; }

		inline auto references() const { return m_references; }
		inline auto & resource() const { return *m_resource; }

	private:
		void increment();
		void decrement();

		TypePtr m_resource;
		Factory m_factory;

		unsigned int m_references = 0u;
	};

	// ...
}

// ...

namespace core
{
	template<typename Type>
	inline Asset<Type>::Reference::Reference(const std::string & name, Asset & asset)
		: m_name(name), m_asset(&asset)
	{
		increment();
	}
	template<typename Type>
	inline Asset<Type>::Reference::Reference(const Reference & other)
		: m_asset(other.m_asset)
	{
		increment();
	}
	template<typename Type>
	inline Asset<Type>::Reference::Reference(Reference && other)
		: m_asset(other.m_asset)
	{
		other.m_asset = nullptr;
	}
	template<typename Type>
	inline Asset<Type>::Reference::~Reference()
	{
		decrement();
	}

	template<typename Type>
	inline typename Asset<Type>::Reference &
		Asset<Type>::Reference::operator=(const Reference & other)
	{
		if (this != &other)
		{
			decrement();
			m_asset = other.m_asset;
			increment();
		}
		return *this;
	}
	template<typename Type>
	inline typename Asset<Type>::Reference &
		Asset<Type>::Reference::operator=(Reference && other)
	{
		if (this != &other)
		{
			decrement();
			m_asset = other.m_asset;
			other.m_asset = nullptr;
		}
		return *this;
	}

	template<typename Type>
	inline typename bool Asset<Type>::Reference::operator==(const Reference & other) const
	{
		return m_asset == other.m_asset;
	}
	template<typename Type>
	inline typename bool Asset<Type>::Reference::operator!=(const Reference & other) const
	{
		return !operator==(other);
	}

	// ...

	template<typename Type>
	inline Asset<Type>::Asset()
		: m_factory([]() { return std::make_unique<Type>(); })
	{}
	template<typename Type>
	inline Asset<Type>::Asset(const Factory & factory)
		: m_factory(factory)
	{}

	template<typename Type>
	inline void Asset<Type>::increment()
	{
		if (m_references++ == 0u)
			m_resource = m_factory();
	}
	template<typename Type>
	inline void Asset<Type>::decrement()
	{
		if (--m_references == 0u)
			m_resource = nullptr;
	}
}