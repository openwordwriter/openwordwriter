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

#ifndef __ABICOLLAB_LOCK__
#define __ABICOLLAB_LOCK__

#ifndef _WIN32
#include <pthread.h>
#endif

namespace abicollab
{

class scoped_lock;

class mutex
{
friend class scoped_lock;

public:
	mutex()
	{
#ifdef _WIN32
		repr = CreateMutex(0, FALSE, 0);
#else
		pthread_mutex_init(&repr, NULL);
#endif
	}

	~mutex()
	{
#ifdef _WIN32
		CloseHandle(repr);
#else
		pthread_mutex_destroy(&repr);
#endif
	}

private:
	// we are noncopyable
	mutex( const mutex& ) = delete;
	const mutex& operator=( const mutex& ) = delete;

#ifdef _WIN32
	HANDLE repr;
#else
	pthread_mutex_t repr;
#endif
};

class scoped_lock
{
public:
	scoped_lock(mutex& mutex)
		: m_mutex(mutex)
	{
#ifdef _WIN32
		WaitForSingleObject(m_mutex.repr, INFINITE);
#else
		pthread_mutex_lock(&m_mutex.repr);
#endif
	}

	~scoped_lock()
	{
#ifdef _WIN32
		ReleaseMutex(m_mutex.repr);
#else
		pthread_mutex_unlock(&m_mutex.repr);
#endif
	}

private:
	// we are noncopyable
	scoped_lock( const scoped_lock& ) = delete;
	const scoped_lock& operator=( const scoped_lock& ) = delete;

	mutex& m_mutex;
};

}

#endif /* __ABICOLLAB_LOCK__ */
