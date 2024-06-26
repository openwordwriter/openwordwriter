/* Copyright (C) 2006-2008 by Marc Maurer <uwog@uwog.net>
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

#ifndef __TCPACCOUNTHANDLER__
#define __TCPACCOUNTHANDLER__

#include <memory>
#include <core/account/xp/AccountHandler.h>

#include "IOServerHandler.h"
#include "TCPBuddy.h"

#define DEFAULT_TCP_PORT 25509  /* log2(e + pi) * 10^4 */

extern AccountHandlerConstructor TCPAccountHandlerConstructor;

class TCPAccountHandler : public AccountHandler
{
public:
	TCPAccountHandler();
	virtual ~TCPAccountHandler();

	// housekeeping
	static UT_UTF8String					getStaticStorageType();
	virtual UT_UTF8String					getStorageType() override
		{ return getStaticStorageType(); }
	virtual UT_UTF8String					getDescription() override;
	virtual UT_UTF8String					getDisplayType() override;

	// dialog management
	virtual void							storeProperties() override;

	// connection management
	virtual ConnectResult					connect() override;
	virtual bool							disconnect() override;
	virtual bool							isOnline() override;

	// user management
	void									addBuddy(BuddyPtr pBuddy);
	virtual BuddyPtr						constructBuddy(const PropertyMap& props) override;
	virtual BuddyPtr						constructBuddy(const std::string& descriptor, BuddyPtr pBuddy) override;
	virtual bool							recognizeBuddyIdentifier(const std::string& identifier) override;
	virtual bool							allowsManualBuddies() override
		{ return false; }
	virtual void							forceDisconnectBuddy(BuddyPtr buddy) override;
	virtual bool							hasPersistentAccessControl() override
		{ return false; }
	virtual bool							defaultShareState(BuddyPtr /*pBuddy*/) override;

	// session management
	virtual bool							allowsSessionTakeover() override
		{ return false; }

	// packet management
	virtual bool							send(const Packet* packet) override;
	virtual bool							send(const Packet*, BuddyPtr pBuddy) override;

	// event management
	void									handleEvent(std::shared_ptr<Session> session_ptr);

private:
	void									_teardownAndDestroyHandler();
	void									_handleMessages(std::shared_ptr<Session> session_ptr);

	// user management
	TCPBuddyPtr								_getBuddy(std::shared_ptr<Session> session_ptr);

	// connection management
	virtual UT_sint32						_getPort(const PropertyMap& props);
	void									_handleAccept(IOServerHandler* pHandler, std::shared_ptr<Session> session);

	boost::asio::io_service						m_io_service;
	boost::asio::io_service::work					m_work;
	std::thread*							m_thread;
	bool									m_bConnected; // TODO: drop this, ask the IO handler
	IOServerHandler*						m_pDelegator;

	std::map<TCPBuddyPtr, std::shared_ptr<Session> >		m_clients; // mapping buddies and their accompanying session
};

#endif /* __TCPACCOUNTHANDLER__ */
