#pragma once

#include <string>
#include <map>

#include "SFML/Network.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

class ServerClient;

using ClientMap = std::map<unsigned short, ServerClient*>;


class Server
{
public:
	Server(const std::string& serverIP = "127.0.0.1",
		const unsigned short serverPort = 50000);
	~Server();

	void	init_connect();
	void	recieve_packets();
	void	update_packets();
	void	send_packets();
	void	update();
	void	run();

private:
	sf::UdpSocket*		m_socket;
	std::string*		m_server_IP;
	unsigned short		m_server_port;

	ClientMap			m_client_map;

	float git_pos;
};