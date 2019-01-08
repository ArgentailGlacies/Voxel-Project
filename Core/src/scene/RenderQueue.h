#pragma once

#include "scene/RenderKey.h"

#include <functional>
#include <vector>

namespace core
{
	/**
		A render task is a simple task which performs some rendering routine. This may be actually
		rendering an object, or it might be changing the OpenGL state in some meaningful way.
	*/
	using RenderTask = std::function<void()>;

	/**
		The render queue will accept any number of rendering tasks to perform. It will ensure that
		the tasks are executed in an ideal order, such that the quality of the final rendered image
		is at its best, and that the rendering performance is the best it can be.
	*/
	class RenderQueue
	{
	public:
		RenderQueue() = default;
		RenderQueue(const RenderQueue &) = delete;
		RenderQueue(RenderQueue &&) = delete;
		~RenderQueue() = default;

		RenderQueue & operator=(const RenderQueue &) = delete;
		RenderQueue & operator=(RenderQueue &&) = delete;

		/**
			Adds the specified rendering task to the queue under the given key. The actual ordering
			of all tasks is undefined; the queue must be sorted before the rendering method is
			invoked.

			@param key The key explaining when the task should be performed.
			@param task The rendering task which should be performed.
		*/
		void add(const RenderKey & key, const RenderTask & task);
		
		/**
			Sorts the rendering queue such that the rendering order is well-defined and ideal.
		*/
		void sort();
		/**
			Actually performs all rendering tasks added to the render queue.
		*/
		void render() const;

	private:
		struct Entry
		{
			Entry(const RenderKey::Data & data, const RenderTask & task) : data(data), task(task) {}

			RenderKey::Data data;
			RenderTask task;
		};

		std::vector<Entry> m_entries;
	};
}