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

#ifndef __REALM_CONNECTION__
#define __REALM_CONNECTION__

#include <memory>
#if defined(HAVE_BOOST_ASIO_HPP)
# include <boost/asio.hpp>
#else
# include <asio.hpp>
#endif
#include <vector>
#include <string>
#include "ut_types.h"
#include "RealmBuddy.h"
#include "RealmGrowBuffer.h"
#include "RealmProtocol.h"
#include "tls_tunnel.h"
#include <sync/xp/lock.h>
#include <sync/xp/SynchronizedQueue.h>

class AP_Dialog_GenericProgress;

struct PendingDocumentProperties
{
	PendingDocumentProperties(AP_Dialog_GenericProgress* pDlg_,
								PD_Document** pDoc_, XAP_Frame* pFrame_,
								const std::string& filename_, bool bLocallyOwned_)
	: pDlg(pDlg_),
	pDoc(pDoc_),
	pFrame(pFrame_),
	filename(filename_),
	bLocallyOwned(bLocallyOwned_)
	{}

	AP_Dialog_GenericProgress* pDlg;
	PD_Document** pDoc;
	XAP_Frame* pFrame;
	std::string filename;
	bool bLocallyOwned;
};

class UserJoinedPacket;
typedef std::shared_ptr<realm::protocolv1::UserJoinedPacket> UserJoinedPacketPtr;

class RealmConnection : public std::enable_shared_from_this<RealmConnection>
{
public:
	RealmConnection(const std::string& ca_file, const std::string& address, int port, bool tls,
					const std::string& cookie, UT_uint64 doc_id, bool master, const std::string& session_id,
					std::function<void (std::shared_ptr<RealmConnection>)> sig);

	bool								connect();
	void								disconnect();
	bool								isConnected();

	void								addBuddy(RealmBuddyPtr buddy_ptr);
	void								removeBuddy(UT_uint8 realm_connection_id);
	RealmBuddyPtr						getBuddy(UT_uint8 realm_connection_id);
	std::vector<RealmBuddyPtr>&			getBuddies()
		{ return m_buddies; }

	UT_uint64							user_id()
		{ return m_user_id; }
	UT_uint8							connection_id()
		{ return m_connection_id; }
	UT_uint64							doc_id()
		{ return m_doc_id; }
	bool								master()
		{ return m_master; }
	void								promote();
	const std::string&					session_id()
		{ return m_session_id; }
	PD_Document*						getDocument()
		{ return m_pDoc; }
	void								setDocument(PD_Document* pDoc)
		{ m_pDoc = pDoc; }
	SynchronizedQueue<realm::protocolv1::PacketPtr>&		queue()
		{ return m_packet_queue; }
	boost::asio::ip::tcp::socket&				socket()
		{ return m_socket; }

	void								loadDocumentStart(AP_Dialog_GenericProgress* pDlg,
														  PD_Document** pDoc_, XAP_Frame* pFrame_, const std::string& filename_,
														  bool bLocallyOwned_)
	{
		UT_return_if_fail(!m_pdp_ptr);
		m_pdp_ptr.reset(new PendingDocumentProperties(pDlg, pDoc_, pFrame_, filename_, bLocallyOwned_));
	}
	std::shared_ptr<PendingDocumentProperties>
										getPendingDocProps()
		{ return m_pdp_ptr; }
	void								loadDocumentEnd()
		{ m_pdp_ptr.reset(); }


private:
	void								_disconnect();
	void								_signal();
	bool								_login();
	UserJoinedPacketPtr					_receiveUserJoinedPacket();
	void								_receive();
	void								_message(const boost::system::error_code& e,
												std::size_t bytes_transferred,
												std::shared_ptr<std::string> msg_ptr);

	void								_complete_packet(realm::protocolv1::PacketPtr packet_ptr);
	void								_complete(const boost::system::error_code& e, std::size_t bytes_transferred,
												realm::protocolv1::PacketPtr packet_ptr);

	boost::asio::io_service					m_io_service;
	std::string							m_ca_file;
	std::string							m_address;
	int									m_port;
	int									m_tls;
	boost::asio::ip::tcp::socket				m_socket;
	std::shared_ptr<std::thread>		m_thread_ptr;
	std::string							m_cookie;
	UT_uint64							m_user_id; // only valid after login
	UT_uint8							m_connection_id; // only valid after login
	UT_uint64							m_doc_id;
	bool								m_master;
	std::string							m_session_id;
	PD_Document*						m_pDoc;
	realm::GrowBuffer					m_buf;
	SynchronizedQueue<realm::protocolv1::PacketPtr>		m_packet_queue;
	std::function<void (std::shared_ptr<RealmConnection>)> m_sig;
	std::vector<RealmBuddyPtr>			m_buddies;

	std::shared_ptr<PendingDocumentProperties>
										m_pdp_ptr;
	std::shared_ptr<tls_tunnel::ClientProxy>
										m_tls_tunnel_ptr;

	abicollab::mutex					m_mutex;
};

typedef std::shared_ptr<RealmConnection> ConnectionPtr;

#endif /* __REALM_CONNECTION__ */
