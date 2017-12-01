#pragma once

#include <string>

#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"

// forward
class PlayerClient;
class PlayerServer;


class Client
{
public:
	Client(const std::string& serverIP = "127.0.0.1",
			const unsigned short serverPort = 50000);

	void			update();

private:
	sf::UdpSocket*		m_socket;
	std::string*		m_client_IP;
	std::string*		m_server_IP;
	unsigned short		m_client_port,
						m_server_port;

private:
	sf::RenderWindow*	m_render_win;
	size_t				m_win_width,
						m_win_height;
	sf::Clock*			m_clock;
	float				m_framerate;

private:
	PlayerClient*		m_player_local;
	PlayerServer*		m_player_server;
};
