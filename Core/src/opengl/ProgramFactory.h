#pragma once

#include "asset/AssetUtil.h"
#include "opengl/Program.h"
#include "opengl/UBORegistry.h"

namespace core
{
	class ProgramLoader : public Loader<Program, UBORegistry>
	{
	public:
		virtual std::unique_ptr<Program> load(
			const util::File & file, const UBORegistry & registry
		) override final;

	private:
		void bind(const Program & program, const UBORegistry & registry) const;
	};
}