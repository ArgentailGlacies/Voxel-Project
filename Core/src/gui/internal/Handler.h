#pragma once

#include <functional>

namespace core
{
	struct Widget;

	class Handler
	{
	public:
		using Callback = std::function<void()>;

		// ...

		virtual ~Handler() = default;

		/**
			Invoked whenever the given widget is activated by the user or by the system in some
			manner.

			@param widget The widget which was activated.
		*/
		virtual void action(Widget & widget) = 0;
		
		/**
			Invoked every tick while the program is running. The handler may, if necessary, perform
			some logic on the widget.

			@param widget The widget which is to be processed.
		*/
		virtual void process(Widget & widget) = 0;
	};
}