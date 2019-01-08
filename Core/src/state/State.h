#pragma once

namespace core
{
	class Engine;

	class State
	{
	public:
		/**
			Invoked when the state is being initialized. All state resources should be declared
			at this point in time.
		*/
		virtual void initialize(Engine & engine) = 0;

		/**
			Performs a single update on the state. The number of updates per second depends on the
			engine configuration.
		*/
		virtual void process() = 0;
	};
}