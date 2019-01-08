#pragma once

#include "asset/AssetUtil.h"
#include "util/StringParsing.h"

namespace core
{
	struct MockAsset
	{
		int m_int;
		float m_float;
	};

	class AssetBuilder : public Builder<int, int>
	{
	public:
		virtual std::unique_ptr<int> build(const int & value) override final
		{
			return std::make_unique<int>(value);
		}
	};

	class AssetLoader : public Loader<int>
	{
	public:
		virtual std::unique_ptr<int> load(const util::File & file) override final
		{
			const int value = util::Parser<int>{}.parse(file.read());

			return std::make_unique<int>(value);
		}
	};
}