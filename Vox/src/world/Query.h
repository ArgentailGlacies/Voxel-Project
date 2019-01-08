#pragma once

#include "util/MathTemporaryFile.h"
#include "world/data/WorldQuery.h"

#include <glm/vec3.hpp>

namespace vox
{
	struct Block;

	WorldQuery readBlock(const glm::ivec3 & pos);
	WorldQuery readCylinder(const glm::ivec3 & start, const glm::ivec3 & end, util::Axis axis);
	WorldQuery readEllipse(const glm::ivec3 & start, const glm::ivec3 & end);
	WorldQuery readRectangle(const glm::ivec3 & start, const glm::ivec3 & end);
	WorldQuery readLine(const glm::ivec3 & start, const glm::ivec3 & end);
	WorldQuery readSphere(const glm::ivec3 & center, unsigned int diameter);

	WorldQuery writeBlock(const Block & block, const glm::ivec3 & pos);
	WorldQuery writeCylinder(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end, util::Axis axis);
	WorldQuery writeEllipse(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end);
	WorldQuery writeRectangle(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end);
	WorldQuery writeLine(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end);
	WorldQuery writeSphere(const Block & block, const glm::ivec3 & center, unsigned int diameter);
}