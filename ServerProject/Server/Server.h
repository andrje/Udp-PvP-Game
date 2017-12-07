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

	void	receive_packet();
	void	update_packet();
	void	send_packet();
	void	init_connect();
	void	init_game();
	void	run_game();
	void	run_server();


private:
	enum ClientState { START = 0, GAME, END, IDLE };

private:
	sf::UdpSocket*		m_socket;
	sf::Packet*			m_packet;
	std::string*		m_server_IP;
	sf::IpAddress*		m_sender_IP;
	unsigned short		m_server_port,
						m_sender_port;

	size_t				m_nr_clients_connected;
	sf::Clock*			m_clock;

	ClientMap			m_client_map;
	SocketMsg			m_socket_msg;
};
