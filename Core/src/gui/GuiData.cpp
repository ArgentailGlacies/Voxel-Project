
#include "GuiData.h"

#include "script/Script.h"
#include "util/StringOperations.h"

void core::gui::initializeScript(const Script & script)
{
	script.execute("global " + STATE_BOOL + " = false;");
	script.execute("global " + STATE_FLOAT + " = 0.0f;");
	script.execute("global " + STATE_STRING + " = \"\";");
}

void core::gui::setState(const core::Script & script, bool state)
{
	script.execute(core::gui::STATE_BOOL + " = " + (state ? "true" : "false") + ";");
}
void core::gui::setState(const core::Script & script, float state)
{
	script.execute(core::gui::STATE_FLOAT + " = " + std::to_string(state) + ";");
}
void core::gui::setState(const core::Script & script, const std::string & state)
{
	script.execute(core::gui::STATE_STRING + " = \"" + util::replaceAll(state, "\"", "\\\"") + "\";");
}
