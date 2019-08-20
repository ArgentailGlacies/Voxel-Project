
#include "File.h"

#include <filesystem>
#include <fstream>
#include <plog/Log.h>
#include <unordered_set>

namespace
{
	std::string parse(const std::string & path, std::unordered_set<std::string> & files)
	{
		std::ifstream stream{ path };
		std::stringstream buffer;

		if (stream.is_open())
		{
			for (std::string line; std::getline(stream, line); )
			{
				if (line.find("#include ") == std::string::npos)
					buffer << line << '\n';
				else
				{
					const auto first = line.find_first_of('\"');
					const auto last = line.find_last_of('\"');
					if (first != last)
					{
						const auto child = line.substr(first + 1u, last - first - 1u);
						if (files.find(child) == files.end())
						{
							files.insert(child);
							buffer << parse(child, files) << '\n';
						}
					}
					else
						LOG_WARNING << "Could not parse line '" << line << "'";
				}
			}
		}
		else
			LOG_WARNING << "Could not read from file '" << path << "'";
		return buffer.str();
	}
}

// ...

bool util::File::exists() const
{
	return std::experimental::filesystem::is_regular_file(m_path);
}
bool util::File::erase() const
{
	return std::remove(m_path.c_str()) == 0;
}

std::string util::File::path() const
{
	return m_path;
}
std::string util::File::root() const
{
	const auto end = m_path.find_last_of('/');
	return end == std::string::npos ? "" : m_path.substr(0u, end);
}
std::string util::File::name() const
{
	const auto start = m_path.find_last_of('/');
	const auto end = m_path.find_last_of('.');

	const auto first = start == std::string::npos ? 0u : start + 1u;
	const auto last = end == std::string::npos ? m_path.length() : end;
	return m_path.substr(first, last - first);
}
std::string util::File::extension() const
{
	const auto start = m_path.find_last_of('.');
	return start == std::string::npos ? "" : m_path.substr(start + 1u);
}

std::string util::File::read() const
{
	std::ifstream file{ m_path };
	std::stringstream buffer;

	if (file.is_open())
		buffer << file.rdbuf();
	else
		LOG_WARNING << "Could not read from file '" << m_path << "'";
	return buffer.str();
}
std::string util::File::parse() const
{
	std::unordered_set<std::string> set = { m_path };
	return ::parse(m_path, set);
}

bool util::File::write(const std::string & data) const
{
	std::ofstream stream{ m_path };

	if (stream.is_open())
		stream << data;
	else
		LOG_WARNING << "Could not write to file '" << m_path << "'";
	return stream.is_open();
}
bool util::File::write(const unsigned char * data, unsigned int size) const
{
	std::fstream stream{ m_path, std::ios::out | std::ios::binary };

	if (stream.is_open())
		stream.write(reinterpret_cast<const char *>(data), size);
	else
		LOG_WARNING << "Could not write to file '" << m_path << "'";
	return stream.is_open();
}
