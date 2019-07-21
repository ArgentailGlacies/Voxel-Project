#pragma once

#include <string>

namespace util
{
	class File
	{
	public:
		File() = delete;
		File(const char * path) : m_path(path) {}
		File(const std::string & path) : m_path(path) {}

		inline bool operator==(const File & file) const { return m_path == file.m_path; }
		inline bool operator!=(const File & file) const { return m_path != file.m_path; }

		/**
			Checks if the file exists on the physical disk.

			@return True iff the file was found on the physical disk.
		*/
		bool exists() const;
		/**
			Attempts to remove the file from the physical disk.

			@return True iff the file was removed from the physical disk.
		*/
		bool erase() const;

		/**
			Returns the full filepath to the file, relative to the root folder.

			@return The path to the file, relative from the root folder.
		*/
		std::string path() const;
		/**
			Returns the path to the parent folder, relative to the root folder. If the parent folder
			does not exist, an empty string is returned.

			@return The parent folder path, relative from the root folder.
		*/
		std::string root() const;
		/**
			Returns the name of the file.

			@return The name of the file.
		*/
		std::string name() const;
		/**
			Returns the type of the file. If the file has no extension, an empty string is returned.

			@return The type of the file.
		*/
		std::string extension() const;

		/**
			Reads the contents of the entire file as a string and returns it.

			@return The entire file contents dumped into a string.
		*/
		std::string read() const;
		/**
			Reads the entire file and processes it. If the file contains any lines starting with
			'#include', followed by a valid file path in quotation marks, the file is loaded and
			included in the data string. Files loaded this way will only be included at most once.

			@return The entire file contents processed into a string.
		*/
		std::string parse() const;
		/**
			Writes the given data to the file, overwriting any previous contents.

			@param data The data to write into the file.
			@return True iff the data was written into the file.
		*/
		bool write(const std::string & data) const;
		/**
			Writes the given data to the file, overwriting any previous contents.

			@param data The data to write into the file.
			@param size The number of bytes to write into the file.
			@return True iff the data was written into the file.
		*/
		bool write(const unsigned char * data, unsigned int size) const;

	private:
		std::string m_path;
	};
}

namespace std
{
	template<>
	struct hash<util::File>
	{
		inline size_t operator()(const util::File & file) const { return hash<string>{}(file.path()); }
	};
}