#pragma once

#include "asset/AssetRegistry.h"
#include "io/File.h"
#include "io/Folder.h"
#include "util/StringOperations.h"

#include <functional>
#include <string>

namespace core
{
	template<typename Asset, typename ...Args>
	class Builder
	{
	public:
		/**
			Configures the asset in some meaningful way. The asset should be fully initialized and
			ready for consumption once it has been built.

			@see core::setupBuilder
			@param args The arguments passed in when the builder was set up.
			@return The new asset constructed by the builder.
		*/
		virtual std::unique_ptr<Asset> build(const Args & ...args) = 0;
	};

	template<typename Asset, typename ...Args>
	class Loader
	{
	public:
		/**
			Configures the asset in some meaningful way. The asset should be fully initialized and
			ready for consumption once it has been loaded.

			@see core::setupLoader
			@param file The file containing the asset data
			@param args The arguments passed in when the loader was set up.
			@return The new asset constructed by the loader.
		*/
		virtual std::unique_ptr<Asset> load(const util::File & file, const Args & ...args) = 0;
	};
	
	// ...

	/**
		Registers an asset of the given type under the given name. The asset will be constructed
		by the provided builder, using the provided arguments.

		@param registry The asset registry to store the asset under.
		@param name The name to store the asset under.
		@param args The arguments to pass to the asset builder.
	*/
	template<typename Builder, typename Asset, typename ...Args>
	void setupBuilder(
		AssetRegistry & registry,
		const std::string & name,
		const Args & ...args
	);

	/**
		Registers multiple assets of the given type under the name of their file paths. The assets
		are loaded by the provided loader, using the provided arguments. Any assets found in the 
		sub-folders of the root will also be registered. Only files of the specified file type will
		be considered for registration.

		@param registry The asset registry to store the asset under.
		@param root The rool folder containing the assets to register.
		@param filetype The type of the files of interest.
		@param args The arguments to pass to the asset loader.
	*/
	template<typename Loader, typename Asset, typename ...Args>
	void setupLoader(
		AssetRegistry & registry,
		const util::Folder & root,
		const std::string & filetype,
		const Args & ...args
	);

	namespace detail
	{
		void logSearchingFolder(const util::Folder & folder);
		void logFoundValidAsset(const util::File & file);
		void logCreatedFactory(const std::string & name);

		template<typename Loader, typename Asset, typename ...Args>
		void recursiveLoader(
			AssetRegistry & registry,
			const util::Folder & root,
			const util::Folder & current,
			const std::string & filetype,
			const Args & ...args
		);
	}
}

// ...

namespace core
{
	template<typename Builder, typename Asset, typename ...Args>
	void setupBuilder(
		AssetRegistry & registry,
		const std::string & name,
		const Args & ...args
	)
	{
		const auto factory = [&args...]() { return Builder{}.build(args...); };
		registry.add<Asset>(name, factory);
		detail::logCreatedFactory(name);
	}

	template<typename Loader, typename Asset, typename ...Args>
	void setupLoader(
		AssetRegistry & registry,
		const util::Folder & root,
		const std::string & filetype,
		const Args & ...args
	)
	{
		detail::recursiveLoader<Loader, Asset>(registry, root, root, filetype, args...);
	}
	template<typename Loader, typename Asset, typename ...Args>
	void detail::recursiveLoader(
		AssetRegistry & registry,
		const util::Folder & root,
		const util::Folder & current,
		const std::string & filetype,
		const Args & ...args
	)
	{
		logSearchingFolder(current);
		for (const auto & file : current.files())
		{
			if (file.extension() != filetype)
				continue;
			const auto path = util::replace(current.path(), root.path() + "/", "");
			const auto name = root == current ? file.name() : path + "/" + file.name();
			logFoundValidAsset(file);

			const auto factory = [file, &args...]() { return Loader{}.load(file, args...); };
			registry.add<Asset>(name, factory);
			logCreatedFactory(name);
		}
		for (const auto & folder : current.folders())
			recursiveLoader<Loader, Asset>(registry, root, folder, filetype, args...);
	}
}