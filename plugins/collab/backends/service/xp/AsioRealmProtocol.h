#ifndef __ASIO_REALM_PROTOCOL__
#define __ASIO_REALM_PROTOCOL__

#include "RealmProtocol.h"

namespace realm {

namespace protocolv1 {

	template <typename WriteHandler>
	static void send(const RoutingPacket& p, boost::asio::ip::tcp::socket& socket, WriteHandler handler) {
		std::vector<boost::asio::const_buffer> bufs(4);
		bufs.push_back(boost::asio::buffer(&p.type(), 1));
		bufs.push_back(boost::asio::buffer(&p.getPayloadSize(), 4)); // FIXME: not Big Endian safe!
		bufs.push_back(boost::asio::buffer(&p.getAddressCount(), 1));
		bufs.push_back(boost::asio::buffer(&(p.getConnectionIds()[0]), p.getConnectionIds().size()));
		bufs.push_back(boost::asio::buffer(*p.getMessage()));
		boost::asio::async_write(socket, bufs, handler);
	}

	template <typename WriteHandler>
	static void send(const DeliverPacket& p, boost::asio::ip::tcp::socket& socket, WriteHandler handler) {
		std::vector<boost::asio::const_buffer> bufs(4);
		bufs.push_back(boost::asio::buffer(&p.type(), 1));
		bufs.push_back(boost::asio::buffer(&p.getPayloadSize(), 4)); // FIXME: not Big Endian safe!
		bufs.push_back(boost::asio::buffer(&p.getConnectionId(), 1));
		bufs.push_back(boost::asio::buffer(*p.getMessage()));
		boost::asio::async_write(socket, bufs, handler);
	}

	template <typename WriteHandler>
	static void send(const UserJoinedPacket& p, boost::asio::ip::tcp::socket& socket, WriteHandler handler) {
		std::vector<boost::asio::const_buffer> bufs(4);
		bufs.push_back(boost::asio::buffer(&p.type(), 1));
		bufs.push_back(boost::asio::buffer(&p.getPayloadSize(), 4)); // FIXME: not Big Endian safe!
		bufs.push_back(boost::asio::buffer(&p.getConnectionId(), 1));
		bufs.push_back(boost::asio::buffer(&p.isMaster(), 1));
		bufs.push_back(boost::asio::buffer(*p.getUserInfo()));
		boost::asio::async_write(socket, bufs, handler);
	}

	template <typename WriteHandler>
	static void send(const UserLeftPacket& p, boost::asio::ip::tcp::socket& socket, WriteHandler handler) {
		std::vector<boost::asio::const_buffer> bufs(2);
		bufs.push_back(boost::asio::buffer(&p.type(), 1));
		bufs.push_back(boost::asio::buffer(&p.getConnectionId(), 1));
		boost::asio::async_write(socket, bufs, handler);
	}

	template <typename WriteHandler>
	static void send(const SessionTakeOverPacket& p, boost::asio::ip::tcp::socket& socket, WriteHandler handler) {
		boost::asio::async_write(socket, boost::asio::buffer(&p.type(), 1), handler);
	}

}

}

#endif /* ASIO_REALM_PROTOCOL__ */
