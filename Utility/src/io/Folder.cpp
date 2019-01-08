
#include "Folder.h"

#include "util/StringOperations.h"

#include <filesystem>
#include <plog/Log.h>

bool util::Folder::exists() const
{
	return std::filesystem::is_directory(m_path);
}
bool util::Folder::create(bool nested) const
{
	try
	{
		if (nested)
			return std::filesystem::create_directories(m_path);
		else
			return std::filesystem::create_directory(m_path);
	}
	catch (const std::filesystem::filesystem_error & error)
	{
		LOG_WARNING << error.what();
		return false;
	}
}
bool util::Folder::erase(bool recursive) const
{
	try
	{
		if (recursive)
			return std::filesystem::remove_all(m_path);
		else
			return std::filesystem::remove(m_path);
	}
	catch (const std::filesystem::filesystem_error & error)
	{
		LOG_WARNING << error.what();
		return false;
	}
}

util::File util::Folder::file(const File & file) const
{
	if (m_path.empty())
		return file.path();
	if (file.path().empty())
		return m_path;
	return { m_path + "/" + file.path() };
}
std::vector<util::File> util::Folder::files() const
{
	try
	{
		std::vector<File> files;
		for (auto & item : std::filesystem::directory_iterator(m_path))
		{
			if (std::filesystem::is_regular_file(item))
				files.push_back(replaceAll(item.path().string(), "\\", "/"));
		}
		return files;
	}
	catch (const std::filesystem::filesystem_error & error)
	{
		LOG_WARNING << error.what();
		return {};
	}
}
util::Folder util::Folder::folder(const Folder & folder) const
{
	if (m_path.empty())
		return folder.path();
	if (folder.path().empty())
		return m_path;
	return { m_path + "/" + folder.path() };
}
std::vector<util::Folder> util::Folder::folders() const
{
	try
	{
		std::vector<Folder> folders;
		for (auto & item : std::filesystem::directory_iterator(m_path))
		{
			if (std::filesystem::is_directory(item))
				folders.push_back(replaceAll(item.path().string(), "\\", "/"));
		}
		return folders;
	}
	catch (const std::filesystem::filesystem_error & error)
	{
		LOG_WARNING << error.what();
		return {};
	}
}

std::string util::Folder::path() const
{
	return m_path;
}
std::string util::Folder::root() const
{
	const auto end = m_path.find_last_of('/');
	return end == std::string::npos ? "" : m_path.substr(0u, end);
}
std::string util::Folder::name() const
{
	const auto start = m_path.find_last_of('/');
	return start == std::string::npos ? m_path : m_path.substr(start + 1u);
}
