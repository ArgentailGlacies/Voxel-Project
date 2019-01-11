
#include "Setup.h"

#include "allegro/SpriteFactory.h"
#include "asset/AssetUtil.h"
#include "core/Resources.h"
#include "ecs/ECS.h"
#include "opengl/ProgramFactory.h"
#include "opengl/UBORegistry.h"
#include "scene/RenderComponent.h"
#include "world/render/BlockTextureAtlas.h"

#include <glm/mat4x4.hpp>

void core::setupAssets(AssetRegistry & assets, const util::Folder & data, const UBORegistry & ubos)
{
	// Loading up assets from data folder
	setupLoader<ProgramLoader, Program>(assets, data.folder(res::path::SHADERS), "xml", ubos);
	setupLoader<SpriteLoader, Sprite>(assets, data.folder(res::path::SPRITES), "xml");

	// Building assets from code
	setupBuilder<vox::BlockTextureAtlasBuilder, Texture>(assets, res::asset::BLOCK_TEXTURE_ATLAS);
}

void core::setupComponents(ECS & ecs)
{
	ecs.addComponent<RenderComponent>();
}

void core::setupSystems(ECS & ecs)
{
}

void core::setupUBOs(UBORegistry & ubos)
{
	ubos.add<glm::mat4>(res::ubo::MATRICES, res::ubo::PROJECTION);
	ubos.add<glm::mat4>(res::ubo::MATRICES, res::ubo::VIEW);
	ubos.add<glm::mat4>(res::ubo::MATRICES, res::ubo::MODEL);
	ubos.build(res::ubo::MATRICES);
}
