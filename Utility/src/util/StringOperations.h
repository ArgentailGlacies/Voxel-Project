#pragma once

#include <string>
#include <vector>

namespace util
{
	std::vector<std::string> split(const std::string & string, const std::string & delim = " ");

	std::string replace(std::string string, const std::string & sub, const std::string & rep);
	std::string replaceAll(std::string string, const std::string & sub, const std::string & rep);
}