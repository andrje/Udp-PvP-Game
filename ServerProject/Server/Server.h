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
	enum SocketStatus
	{
		DONE,
		NOTREADY,
		DISCONNECTED,
		ERROR
	}
	m_status_type;

public:
	Server(const std::string& serverIP = "127.0.0.1",
		const unsigned short serverPort = 50000);
	~Server();

	bool	update_tick(const float tickRate);
	void	recieve_packets();
	void	update_packets();
	void	send_packets();
	void	init_connect();
	void	run_connect();
	int		socket_status(const char socketTransferType,
						sf::Socket::Status& status);
	void	run();

private:
	sf::UdpSocket*		m_socket;
	sf::Socket::Status	m_socket_status;
	std::string*		m_server_IP;
	unsigned short		m_server_port;

	size_t				m_nr_clients_connected;

	ClientMap			m_client_map;
	SocketMsg			m_socket_msg;

	sf::Clock*			m_clock;
	float				m_tickrate,
						m_current_t,
						m_last_t;
};