
#include "Side.h"

#include "util/StringOperations.h"

#include <plog/Log.h>

const vox::Side vox::Side::POS_X{
	"pos_x", 0, NEG_X, { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 1, 2 }
};
const vox::Side vox::Side::POS_Y{
	"pos_y", 1, NEG_Y, { 0, 1, 0 }, { -1, 0, 0 }, { 0, 0, 1 }, { 1, 0, 2 }
};
const vox::Side vox::Side::POS_Z{
	"pos_z", 2, NEG_Z, { 0, 0, 1 }, { 0, -1, 0 }, { 1, 0, 0 }, { 2, 1, 0 }
};
const vox::Side vox::Side::NEG_X{
	"neg_x", 3, POS_X, { -1, 0, 0 }, { 0, -1, 0 }, { 0, 0, 1 }, { 0, 1, 2 }
};
const vox::Side vox::Side::NEG_Y{
	"neg_y", 4, POS_Y, { 0, -1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 1, 0, 2 }
};
const vox::Side vox::Side::NEG_Z{
	"neg_z", 5, POS_Z, { 0, 0, -1 }, { 0, -1, 0 }, { 1, 0, 0 }, { 2, 1, 0 }
};
const vox::Side vox::Side::OTHER{
	"other", 6, OTHER, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 1, 2 }
};

const vox::Side vox::Side::AXIS[3] = {
	POS_X, POS_Y, POS_Z
};
const vox::Side vox::Side::SIDES[4] = {
	POS_X, POS_Y, NEG_X, NEG_Y
};
const vox::Side vox::Side::FACES[6] = {
	POS_X, POS_Y, POS_Z, NEG_X, NEG_Y, NEG_Z
};
const vox::Side vox::Side::ALL[7] = {
	POS_X, POS_Y, POS_Z, NEG_X, NEG_Y, NEG_Z, OTHER
};

const vox::Side & vox::Side::fromName(const std::string & name)
{
	if (name == "pos_x") return POS_X;
	if (name == "pos_y") return POS_Y;
	if (name == "pos_z") return POS_Z;
	if (name == "neg_x") return NEG_X;
	if (name == "neg_y") return NEG_Y;
	if (name == "neg_z") return NEG_Z;
	if (name == "other") return OTHER;
	LOG_WARNING << "Invalid side " << name;
	return OTHER;
}
std::vector<vox::Side> vox::Side::fromList(const std::string & list)
{
	bool include[7] = { false };
	for (const auto & part : util::split(list, ", "))
	{
		if (part == "all")
		{
			for (const auto & side : ALL)
				include[side.id()] = true;
		}
		else if (part == "faces")
		{
			for (const auto & side : FACES)
				include[side.id()] = true;
		}
		else if (part == "sides")
		{
			for (const auto & side : SIDES)
				include[side.id()] = true;
		}
		else
			include[fromName(part).id()] = true;
	}
	
	std::vector<vox::Side> sides;
	for (const auto & side : ALL)
	{
		if (include[side.id()])
			sides.push_back(side);
	}
	return sides;
}
