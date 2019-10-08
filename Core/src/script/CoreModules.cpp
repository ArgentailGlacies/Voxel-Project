
#include "CoreModules.h"

#include "gui/Gui.h"
#include "gui/GuiRegistry.h"
#include "gui/Widget.h"
#include "io/File.h"
#include "io/Folder.h"
#include "script/Script.h"
#include "script/ScriptUtil.h"
#include "util/MathTemporaryFile.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace core
{
	std::string WIDGET;

	// ...

	template<int L, typename T>
	void bindVec(Script & script, const std::string & name)
	{
		addType<glm::vec<L, T>>(script, name);
		addCtor<glm::vec<L, T>()>(script, name);
		addCtor<glm::vec<L, T>(glm::vec<L, T>)>(script, name);

		addFunction(script, [](glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 += v2; }, "+=");
		addFunction(script, [](glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 -= v2; }, "-=");
		addFunction(script, [](glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 *= v2; }, "*=");
		addFunction(script, [](glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 /= v2; }, "/=");
		addFunction(script, [](glm::vec<L, T> & v, T scalar) { return v += scalar; }, "+=");
		addFunction(script, [](glm::vec<L, T> & v, T scalar) { return v -= scalar; }, "-=");
		addFunction(script, [](glm::vec<L, T> & v, T scalar) { return v *= scalar; }, "*=");
		addFunction(script, [](glm::vec<L, T> & v, T scalar) { return v /= scalar; }, "/=");
		addFunction(script, [](const glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 + v2; }, "+");
		addFunction(script, [](const glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 - v2; }, "-");
		addFunction(script, [](const glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 * v2; }, "*");
		addFunction(script, [](const glm::vec<L, T> & v1, const glm::vec<L, T> & v2) { return v1 / v2; }, "/");
		addFunction(script, [](const glm::vec<L, T> & v, T scalar) { return v + scalar; }, "+");
		addFunction(script, [](const glm::vec<L, T> & v, T scalar) { return v - scalar; }, "-");
		addFunction(script, [](const glm::vec<L, T> & v, T scalar) { return v * scalar; }, "*");
		addFunction(script, [](const glm::vec<L, T> & v, T scalar) { return v / scalar; }, "/");
		addFunction(script, [](T scalar, const glm::vec<L, T> & v) { return scalar + v; }, "+");
		addFunction(script, [](T scalar, const glm::vec<L, T> & v) { return scalar - v; }, "-");
		addFunction(script, [](T scalar, const glm::vec<L, T> & v) { return scalar * v; }, "*");
		addFunction(script, [](T scalar, const glm::vec<L, T> & v) { return scalar / v; }, "/");
	}
	template<typename T>
	void bindVec2(Script & script, const std::string & name)
	{
		bindVec<2, T>(script, name);
		addCtor<glm::tvec2<T>(T, T)>(script, name);
	}
	template<typename T>
	void bindVec3(Script & script, const std::string & name)
	{
		bindVec<3, T>(script, name);
		addCtor<glm::tvec3<T>(T, T, T)>(script, name);
	}
	template<typename T>
	void bindVec4(Script & script, const std::string & name)
	{
		bindVec<4, T>(script, name);
		addCtor<glm::tvec4<T>(T, T, T, T)>(script, name);
	}
}

// ...

void core::ModuleMath::bind(Script & script) const
{
	addGlobalConstant(script, util::Axis::X, "AXIS_X");
	addGlobalConstant(script, util::Axis::Y, "AXIS_Y");
	addGlobalConstant(script, util::Axis::Z, "AXIS_Z");

	bindVec2<float>(script, "vec2");
	bindVec3<float>(script, "vec3");
	bindVec4<float>(script, "vec4");
	bindVec2<int>(script, "ivec2");
	bindVec3<int>(script, "ivec3");
	bindVec4<int>(script, "ivec4");
	bindVec2<unsigned int>(script, "uvec2");
	bindVec3<unsigned int>(script, "uvec3");
	bindVec4<unsigned int>(script, "uvec4");
}

void core::ModuleFileSystem::bind(Script & script) const
{
	addType<util::File>(script, "File");
	addCtor<util::File()>(script, "File");
	addCtor<util::File(const char *)>(script, "File");
	addCtor<util::File(const std::string &)>(script, "File");

	addType<util::Folder>(script, "Folder");
	addCtor<util::Folder(const char *)>(script, "Folder");
	addCtor<util::Folder(const std::string &)>(script, "Folder");
}

void core::ModuleGui::bind(Script & script, GuiRegistry & guis, Gui & gui) const
{
	addGlobalVariable(script, &guis, "GUI_REGISTRY");
	addGlobalVariable(script, &gui, "GUI");
	addGlobalVariable(script, &WIDGET, "WIDGET");

	addMethod(script, &Gui::has, "has");
	addMethod(script, &Gui::isVisible, "isVisible");
	addMethod(script, &Gui::setVisible, "setVisible");
	addMethod(script, &Gui::isLocked, "isLocked");
	addMethod(script, &Gui::setLocked, "setLocked");
	addMethod(script, &Gui::getBool, "getBool");
	addMethod(script, &Gui::getFloat, "getFloat");
	addMethod(script, &Gui::getString, "getString");
	addMethod(script, &Gui::setBool, "setBool");
	addMethod(script, &Gui::setFloat, "setFloat");
	addMethod(script, &Gui::setString, "setString");

	addMethod(script, &GuiRegistry::open, "open");
	addMethod(script, &GuiRegistry::close, "close");
}
