#pragma once

namespace core
{
	class Allegro
	{
	public:
		Allegro();
		~Allegro();

		inline auto isInitialized() const { return m_initialized; }
		inline auto hasErrors() const { return m_errors; }

	private:
		bool m_initialized = false;
		bool m_errors = false;
	};
}