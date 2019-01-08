#pragma once

#include "world/render/BlockModel.h"
#include "world/render/BlockTexture.h"

namespace vox
{
	struct BlockRender
	{
		BlockModel m_model;
		BlockTexture m_texture[7];

		bool m_occlude[7] = { true, true, true, true, true, true, true };
	};
}