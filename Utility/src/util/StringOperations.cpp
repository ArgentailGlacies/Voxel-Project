
#include "StringOperations.h"

#include <sstream>
#include <iterator>

std::vector<std::string> util::split(const std::string & string, const std::string & delim)
{
	std::vector<std::string> elements;

	for (size_t current = 0u, next; true; current = next + delim.size())
	{
		next = string.find_first_of(delim, current);
		elements.push_back(string.substr(current, next - current));

		if (next == std::string::npos)
			break;
	}
	return elements;
}

std::string util::replace(std::string string, const std::string & sub, const std::string & rep)
{
	if (string.length() < sub.length() || sub.empty())
		return string;
	const std::size_t pos = string.find(sub);
	if (pos == std::string::npos)
		return string;
	const std::size_t subSize = sub.length();
	string.replace(pos, subSize, rep);
	return string;
}
std::string util::replaceAll(std::string string, const std::string & sub, const std::string & rep)
{
	if (string.length() < sub.length() || sub.empty())
		return string;

	const std::size_t subSize = sub.length();
	const std::size_t repSize = rep.length();

	for (std::size_t pos = 0; ; pos += repSize)
	{
		pos = string.find(sub, pos);
		if (pos == std::string::npos)
			break;
		if (subSize == repSize)
			string.replace(pos, subSize, rep);
		else
		{
			string.erase(pos, subSize);
			string.insert(pos, rep);
		}
	}
	return string;
}
