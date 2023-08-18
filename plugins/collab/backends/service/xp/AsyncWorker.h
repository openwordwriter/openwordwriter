/* Copyright (C) 2008 AbiSource Corporation B.V.
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

#ifndef __ASYNC_WORKER__
#define __ASYNC_WORKER__

#if defined(HAVE_BOOST_ASIO_HPP)
# include <boost/asio.hpp>
#else
# include <asio.hpp>
#endif
#include <memory>

#include <boost/bind/bind.hpp>
#include <boost/function.hpp>
#include "ut_debugmsg.h"
#include <sync/xp/Synchronizer.h>

template <class T>
class AsyncWorker : public std::enable_shared_from_this<AsyncWorker<T> >
{
public:
	AsyncWorker(boost::function<T ()> async_func, boost::function<void (T)> async_callback)
	: m_async_func(async_func),
	m_async_callback(async_callback),
	m_synchronizer() // can't initialize the synchronizer here yet, because you can't call shared_from_this() from a constructor
	{
	}

	AsyncWorker(const AsyncWorker&) = delete;
	AsyncWorker& operator=(const AsyncWorker&) = delete;

	virtual ~AsyncWorker()
	{
		UT_DEBUGMSG(("~AsyncWorker()\n"));
		if (m_thread_ptr)
			m_thread_ptr->join();
	}

	virtual void start()
	{
		m_synchronizer.reset(new Synchronizer(boost::bind(&AsyncWorker<T>::_signal,
												std::enable_shared_from_this<AsyncWorker<T> >::shared_from_this())));
		m_thread_ptr.reset(
				new std::thread(
					boost::bind(&AsyncWorker::_thread_func,
								AsyncWorker<T>::shared_from_this())
				)
			);
	}

private:
	void _signal()
	{
		UT_DEBUGMSG(("Calling async callback function from the main loop\n"));
		m_async_callback(m_func_result);
	}

	void _thread_func()
	{
		UT_DEBUGMSG(("Starting async function...\n"));
		m_func_result = m_async_func();
		UT_DEBUGMSG(("Async function completed...\n"));
		m_synchronizer->signal();
	}

	boost::function<T ()>					m_async_func;
	boost::function<void (T)>				m_async_callback;
	std::shared_ptr<Synchronizer>			m_synchronizer;
	std::shared_ptr<std::thread>			m_thread_ptr;
	T										m_func_result;
};

#endif /* __ASYNC_WORKER__ */
