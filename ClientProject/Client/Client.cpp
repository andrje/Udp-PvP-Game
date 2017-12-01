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
	m_framerate(0.0333)
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

	m_player_local = new PlayerClient();
	m_player_server = new PlayerServer();
}


// update
void Client::update()
{
	float delta_t = 0, current_t = 0, last_t = 0, frame_t = 0;
	m_clock->restart();
	last_t = m_clock->getElapsedTime().asSeconds();

	while (m_render_win->isOpen())
	{
		sf::Event event;
		while (m_render_win->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_render_win->close();
		}

		current_t = m_clock->getElapsedTime().asSeconds();
		delta_t += current_t - last_t;

		if (current_t - frame_t > m_framerate)
		{
			m_render_win->clear(sf::Color::Cyan);

			m_player_server->update();
			m_player_local->update(delta_t,
									*m_socket, 
									*m_server_IP,
									m_server_port,
									*m_render_win);

			m_player_server->render(*m_render_win);
			m_player_local->render(*m_render_win);

			m_render_win->display();

			frame_t = current_t;
			delta_t = 0;
		}

		last_t = current_t;
	}
}
