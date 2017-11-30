#include <iostream>

#include "Client.h"

// forward
#include "../Player/PlayerClient/PlayerClient.h"
#include "../Player/PlayerServer/PlayerServer.h"


// cTor
Client::Client(const std::string& serverIP,
				const unsigned short serverPort)
	: m_socket(new sf::UdpSocket()),
	m_client_IP(new std::string("127.0.0.1")),
	m_server_IP(new std::string(serverIP)),
	m_server_port(serverPort),
	m_win_width(640),
	m_win_height(360),
	m_clock(new sf::Clock()),
	m_framerate(0.0083)	// about 120 fps
{
	m_socket->bind(sf::Socket::AnyPort);
	m_socket->setBlocking(true);
	m_client_port = m_socket->getLocalPort();

	sf::ContextSettings AA;
	AA.antialiasingLevel = 8;
	m_render_win = new sf::RenderWindow(sf::VideoMode(m_win_width, m_win_height),
										"Skjut som fan 2.0",
										sf::Style::Default,
										AA);

	m_player_client = new PlayerClient();
	m_player_server = new PlayerServer();
}


// get win size
sf::Vector2f Client::get_win_size()
{
	return sf::Vector2f(m_win_width, m_win_height);
}


// update
void Client::update()
{
	float deltaT = 0, currentT = 0, lastT = 0, frameT = 0;
	m_clock->restart();
	lastT = m_clock->getElapsedTime().asSeconds();

	while (m_render_win->isOpen())
	{
		sf::Event event;
		while (m_render_win->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_render_win->close();
		}

		currentT = m_clock->getElapsedTime().asSeconds();
		deltaT += currentT - lastT;

		if (currentT - frameT > m_framerate)
		{
			m_render_win->clear(sf::Color::Cyan);

			m_player_client->update(deltaT,
									*m_socket, 
									*m_server_IP,
									m_server_port,
									*m_render_win);
			m_player_server->update();
			m_player_client->render(*m_render_win);
			m_player_server->render(*m_render_win);

			m_render_win->display();

			frameT = currentT;
			deltaT = 0;
		}

		lastT = currentT;
	}
}
