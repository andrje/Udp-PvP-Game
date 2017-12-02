#pragma once

#include <string>
#include <map>
#include <vector>

#include "SFML/Network.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

// forward
class ServerClient;
enum SocketStatus;
// typedef
using ClientMap = std::map<unsigned short, ServerClient*>;
using SocketMsg = std::vector<std::string*>;


class Server
{
public:
	Server(const std::string& serverIP = "127.0.0.1", const unsigned short serverPort = 50000);
	~Server();

	void	recieve_packet();
	void	update_packet();
	void	send_packet();
	void	packet_status(const char socketTransferType, sf::Socket::Status& status);
	void	init_connect();
	bool	update_tick(const float tickRate);
	void	run_connect();
	void	run();

private:
	sf::UdpSocket*		m_socket;
	sf::Socket::Status	m_socket_status;
	sf::Packet*			m_packet;
	std::string*		m_server_IP;
	sf::IpAddress*		m_sender_IP;
	unsigned short		m_server_port,
						m_sender_port;

	size_t				m_nr_clients_connected;

	ClientMap			m_client_map;
	SocketMsg			m_socket_msg;
};