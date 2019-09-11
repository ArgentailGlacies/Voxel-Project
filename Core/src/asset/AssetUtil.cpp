
#include "AssetUtil.h"

#include <plog/Log.h>

void core::detail::logSearchingFolder(const util::Folder & folder)
{
	LOG_DEBUG << "Searching '" << folder.path() << "'...";
}
void core::detail::logFoundValidAsset(const util::File & file)
{
	LOG_DEBUG << "Found '" << file.path() << "'...";
}
void core::detail::logCreatedFactory(const std::string & name)
{
	LOG_INFO << "Created factory for '" << name << "'!";
}