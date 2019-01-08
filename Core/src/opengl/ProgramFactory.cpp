
#include "ProgramFactory.h"

#include "opengl/OpenGL.h"
#include "opengl/Shader.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

namespace
{
	constexpr const char * ATTRIBUTE_TYPE = "type";
	constexpr const char * ATTRIBUTE_FILE = "file";

	const std::string TYPE_FRAGMENT = "fragment";
	const std::string TYPE_GEOMETRY = "geometry";
	const std::string TYPE_VERTEX = "vertex";
}

std::unique_ptr<core::Program> core::ProgramLoader::load(
	const util::File & file, const UBORegistry & registry
)
{
	LOG_INFO << "Loading program " << file.path() << "...";
	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	auto program = std::make_unique<Program>();
	for (auto child = doc.first_child(); child; child = child.next_sibling())
	{
		const std::string attrType = child.attribute(ATTRIBUTE_TYPE).as_string();
		const util::File attrFile = child.attribute(ATTRIBUTE_FILE).as_string();

		ShaderType type = ShaderType::VERTEX;
		if (attrType == TYPE_FRAGMENT)
			type = ShaderType::FRAGMENT;
		else if (attrType == TYPE_GEOMETRY)
			type = ShaderType::GEOMETRY;
		else if (attrType == TYPE_VERTEX)
			type = ShaderType::VERTEX;
		else
			LOG_WARNING << "Invalid shader type " << attrType;

		const auto targetPath = attrFile.exists() ? attrFile.path() : file.path();
		const auto targetCode = attrFile.exists() ? attrFile.parse() : child.child_value();

		Shader shader{ type };
		if (shader.compile(targetCode) && program->attach(shader))
			LOG_INFO << "Compiled and attached " << attrType << " shader from " << targetPath;
		else
			LOG_INFO << "Failed compiling/attaching " << attrType << " shader from " << targetPath;
	}

	if (program->link())
		bind(*program, registry);
	else
		LOG_WARNING << "Failed linking program";
	return program;
}

void core::ProgramLoader::bind(const Program & program, const UBORegistry & registry) const
{
	for (const auto & [name, port] : registry.bindings())
	{
		const auto index = glGetUniformBlockIndex(program.handle(), name.c_str());
		if (index == GL_INVALID_INDEX)
			LOG_WARNING << "Failed binding ubo " << name;
		else
			glUniformBlockBinding(program.handle(), index, port);
		HAS_GL_ERROR;
	}
}