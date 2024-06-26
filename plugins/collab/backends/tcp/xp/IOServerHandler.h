/* Copyright (C) 2007-2008 by Marc Maurer <uwog@uwog.net>
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

#ifndef __IO_SERVER_HANDLER__
#define __IO_SERVER_HANDLER__

#include "ut_debugmsg.h"

#include <functional>

#include <boost/bind/bind.hpp>
#if defined(HAVE_BOOST_ASIO_HPP)
# include <boost/asio.hpp>
#else
# include <asio.hpp>
#endif

#include <sync/xp/Synchronizer.h>
#include "Session.h"

class TCPAccountHandler;

class IOServerHandler
{
public:
	IOServerHandler(int port, std::function<void (IOServerHandler*, std::shared_ptr<Session>)> af,
					std::function<void (std::shared_ptr<Session>)> ef, boost::asio::io_service& io_service_)
	:	accept_synchronizer(std::bind(&IOServerHandler::_signal, this)),
		io_service(io_service_),
		m_pAcceptor(NULL),
		session_ptr(),
		m_af(af),
		m_ef(ef)
	{
 		UT_DEBUGMSG(("IOServerHandler()\n"));
 		m_pAcceptor = new boost::asio::ip::tcp::acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
	}

	virtual ~IOServerHandler()
	{
		UT_DEBUGMSG(("IOServerHandler::~IOServerHandler()\n"));
 		if (m_pAcceptor)
 			stop();
	}

	virtual void stop()
	{
 		UT_DEBUGMSG(("IOServerHandler::stop()\n"));
		if (m_pAcceptor)
		{
			m_pAcceptor->close();
			DELETEP(m_pAcceptor);
		}
	}

	void run()
	{
		UT_DEBUGMSG(("IOServerHandler::run()\n"));
		asyncAccept();
	}

	void asyncAccept()
	{
 		UT_DEBUGMSG(("IOServerHandler::asyncAccept()\n"));
 		UT_return_if_fail(m_pAcceptor);
 		session_ptr.reset(new Session(io_service, m_ef));
		m_pAcceptor->async_accept(session_ptr->getSocket(),
			boost::bind(&IOServerHandler::handleAsyncAccept,
				this, boost::asio::placeholders::error));
	}

private:
	void _signal()
	{
		UT_DEBUGMSG(("IOServerHandler::_signal()\n"));
		UT_return_if_fail(session_ptr);
		session_ptr->asyncReadHeader();
		m_af(this, session_ptr);
	}

	void handleAsyncAccept(const boost::system::error_code& ec)
	{
		UT_DEBUGMSG(("IOServerHandler::handleAsyncAccept()\n"));
		if (ec)
		{
			UT_DEBUGMSG(("Error accepting connection: %s\n", ec.message().c_str()));
			return;
		}
		accept_synchronizer.signal();
	}

	Synchronizer				accept_synchronizer;
	boost::asio::io_service&			io_service;
	boost::asio::ip::tcp::acceptor*	m_pAcceptor;
	std::shared_ptr<Session>	session_ptr;

	std::function<void (IOServerHandler*, std::shared_ptr<Session>)> m_af;
	std::function<void (std::shared_ptr<Session>)> m_ef;
};

#endif /* __IO_SERVER_HANDLER__ */
