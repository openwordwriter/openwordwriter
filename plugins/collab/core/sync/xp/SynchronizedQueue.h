/* Copyright (C) 2008 by Marc Maurer <uwog@uwog.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef __SYNCHRONIZED_QUEUE__
#define __SYNCHRONIZED_QUEUE__

#include <deque>
#include <functional>

#include <sync/xp/lock.h>
#include <sync/xp/Synchronizer.h>

class EmptyQueueException {};

template <typename T>
class SynchronizedQueue : public Synchronizer
{
public:
	SynchronizedQueue(std::function<void (SynchronizedQueue&)> sig)
		: Synchronizer(std::bind(&SynchronizedQueue::_signal, this)),
		m_mutex(),
		m_queue(),
		m_sig(sig)
	{}

	SynchronizedQueue(const SynchronizedQueue&) = delete;
	SynchronizedQueue& operator=(const SynchronizedQueue&) = delete;

	void push(T t)
	{
		abicollab::scoped_lock lock(m_mutex);
		m_queue.push_back( t );
		Synchronizer::signal();
	}

	T pop()
	{
		if (m_queue.size() == 0)
			throw EmptyQueueException();
		abicollab::scoped_lock lock(m_mutex);
		T t = m_queue.front();
		m_queue.pop_front();
		return t;
	}

	bool peek()
	{
		return m_queue.size() > 0;
	}

private:
	void _signal()
	{
		m_sig(*this);
	}

	abicollab::mutex m_mutex;
	std::deque< T > m_queue;
	std::function<void (SynchronizedQueue&)> m_sig;
};

#endif /* __SYNCHRONIZED_QUEUE__ */
