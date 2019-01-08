#pragma once

#include "io/File.h"

#include <string>
#include <vector>

namespace util
{
	class Folder
	{
	public:
		Folder() = delete;
		Folder(const char * path) : m_path(path) {}
		Folder(const std::string & path) : m_path(path) {}

		inline bool operator==(const Folder & folder) const { return m_path == folder.m_path; }
		inline bool operator!=(const Folder & folder) const { return m_path != folder.m_path; }

		/**
			Checks if the folder exists on the physical disk.

			@return True iff the folder was found on the physical disk.
		*/
		bool exists() const;
		/**
			Attempts to create the folder on the physical disk. If specified, the folder will create
			parent folders if they are missing as well.

			@return True iff the folder was created on the physical disk.
		*/
		bool create(bool nested = false) const;
		/**
			Attempts to remove the folder from the physical disk. If specified, the folder will be
			deleted recursively, erasing all the contents of the folder as well.

			@param recursive Whether all sub-folders and files should be deleted too.
			@return True iff the folder was removed from the physical disk.
		*/
		bool erase(bool recursive = false) const;

		/**
			Retrieves the file with the specified path relative to the current folder.

			@param The file specified relative to the folder.
			@return The file with the given name, in the current folder.
		*/
		File file(const File & file) const;
		/**
			Searches through the entire folder for all files stored within it.

			@return A collection of all sub-files within the folder.
		*/
		std::vector<File> files() const;
		/**
			Retrieves the folder with the specified path relative to the current folder.

			@param The name of the folder relative to the folder.
			@return The folder with the given name, in the current folder.
		*/
		Folder folder(const Folder & folder) const;
		/**
			Searches through the entire folder for all folders stored within it.

			@return A collection of all sub-folders within the folder.
		*/
		std::vector<Folder> folders() const;

		/**
			Returns the full filepath to the folder, relative to the root folder.

			@return The path to the folder, relative from the root folder.
		*/
		std::string path() const;
		/**
			Returns the path to the parent folder, relative to the root folder. If the parent folder
			does not exist, an empty string is returned.

			@return The parent folder path, relative from the root folder.
		*/
		std::string root() const;
		/**
			Returns the name of the folder.

			@return The name of the folder.
		*/
		std::string name() const;

	private:
		std::string m_path;
	};
}