#pragma once

#include <string>
#include <map>

#include "SFML/Network.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

// forward
class ServerClient;
// typedef
using ClientMap = std::map<unsigned short, ServerClient*>;


class Server
{
public:
	Server(const std::string& serverIP = "127.0.0.1",
		const unsigned short serverPort = 50000);
	~Server();

	void	recieve_packets();
	void	update_packets();
	void	send_packets();
	void	init_connect();
	void	run_connect();
	void	run();

private:
	sf::UdpSocket*		m_socket;
	std::string*		m_server_IP;
	unsigned short		m_server_port;

	size_t				m_nr_clients_connected;

	ClientMap			m_client_map;

	sf::Clock*			m_clock;
	float				m_tickrate;
};