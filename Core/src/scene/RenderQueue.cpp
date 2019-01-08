
#include "RenderQueue.h"

#include <algorithm>

void core::RenderQueue::add(const RenderKey & key, const RenderTask & task)
{
	m_entries.emplace_back(key.compile(), task);
}

void core::RenderQueue::sort()
{
	std::sort(m_entries.begin(), m_entries.end(), [](const Entry & lhs, const Entry & rhs)
	{
		return lhs.data < rhs.data;
	});
}
void core::RenderQueue::render() const
{
	for (const auto & it : m_entries)
		it.task();
}
