#pragma once

#include <string>

namespace core
{
	class Script;

	namespace gui
	{
		/* Represents the name of the global storing the current widget's boolean state */
		const std::string STATE_BOOL = "STATE_BOOL";
		/* Represents the name of the global storing the current widget's floating point state */
		const std::string STATE_FLOAT = "STATE_FLOAT";
		/* Represents the name of the global storing the current widget's textual state */
		const std::string STATE_STRING = "STATE_STRING";

		// ...

		/**
			The slider's valid values are determined fully be the slider's data structure. The value
			must be within the range [min, max], where the glider is considered in the middle at the
			value 'center'. The value must be a multiple of the step size.
		*/
		struct SliderData
		{
			float m_min = 0.0f;
			float m_max = 1.0f;
			float m_center = 0.5f;
			float m_step = 0.1f;
		};

		// ...

		/**
			Initializes the script by assigning all relevant global fields which are required for a
			gui script to function as intended. The script will after initialization contain global
			variables holding the state of the last widget click and data referencing which gui the
			script belongs to.

			@param script The script to initialize.
		*/
		void initializeScript(const Script & script);

		/**
			Assigns the boolean state of the widget to the given script.

			@param script The script which the update should be performed on.
			@param state The new state value.
		*/
		void setState(const core::Script & script, bool state);
		/**
			Assigns the float state of the widget to the given script.

			@param script The script which the update should be performed on.
			@param state The new state value.
		*/
		void setState(const core::Script & script, float state);
		/**
			Assigns the string state of the widget to the given script.

			@param script The script which the update should be performed on.
			@param state The new state value.
		*/
		void setState(const core::Script & script, const std::string & state);
	}
}
