#pragma once

#include <functional>

namespace core
{
	class MainLoop
	{
	public:
		using Processor = std::function<void(double, double)>;
		using Renderer = std::function<void(float)>;

		MainLoop() = delete;
		MainLoop(const Processor & processor, const Renderer & renderer);
		MainLoop(const MainLoop &) = delete;
		MainLoop(MainLoop &&) = delete;
		~MainLoop() = default;

		MainLoop & operator=(const MainLoop &) = delete;
		MainLoop & operator=(MainLoop &&) = delete;

		void start(double ups, double fps);
		void stop();

	private:
		Processor m_processor;
		Renderer m_renderer;

		bool m_running = false;
	};
}