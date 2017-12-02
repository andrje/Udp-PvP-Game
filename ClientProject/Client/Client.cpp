#include <iostream>

#include "Client.h"

// forward
#include "../Player/PlayerClient/PlayerClient.h"
#include "../Player/PlayerServer/PlayerServer.h"


// cTor
Client::Client(const std::string& serverIP, const unsigned short serverPort)
	:
	m_socket(new sf::UdpSocket()),
	m_packet(new sf::Packet()),
	m_client_IP(new std::string("127.0.0.1")),
	m_server_IP(new std::string(serverIP)),
	m_server_port(serverPort),
	m_win_width(640),
	m_win_height(360),
	m_clock(new sf::Clock()),
	m_tickrate(0.0167),		// 30 = 0.0333 | 60 = 0.0167 | 120 = 0.0083
	m_framerate(0.0167),
	m_last_t_tick(0),
	m_last_t_frame(0),
	m_delta_t(0),
	m_do_tick(false),
	m_do_frame(false)
{
	m_socket->bind(sf::Socket::AnyPort);
	m_socket->setBlocking(false);
	m_client_port = m_socket->getLocalPort();

	sf::ContextSettings AA;
	AA.antialiasingLevel = 8;
	m_render_win = new sf::RenderWindow(sf::VideoMode(m_win_width, m_win_height),
										"Skjut som fan 2.0",
										sf::Style::Default,
										AA);

	m_socket_msg.push_back(new std::string("done"));
	m_socket_msg.push_back(new std::string("not ready"));
	m_socket_msg.push_back(new std::string("disconnected"));
	m_socket_msg.push_back(new std::string("error"));

	m_player_local = new PlayerClient();
	m_player_server = new PlayerServer();
}


// send packet
void Client::send_packet()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_socket_status = m_socket->send(*m_player_local->get_packet(),
										*m_server_IP,
										m_server_port);
	}

	

	packet_status('s', m_socket_status);	// 's' is key for (s)end messages
}


// receive packet
void Client::receive_packet()
{
	sf::IpAddress sender_IP;
	unsigned short sender_port;

	m_packet->clear();
	m_socket_status = m_socket->receive(*m_packet, sender_IP, sender_port);

	packet_status('r', m_socket_status);	// 'r' is key for (r)ecieve messages
}


// packet_status
void Client::packet_status(const char socketTransferType, sf::Socket::Status& status)
{
	switch (status)
	{
		// Done
	case sf::Socket::Done:

		if (socketTransferType == 'r')
			m_player_local->set_packet(*m_packet);

		break;
		// NotReady
	case sf::Socket::NotReady:
		break;
		// Disconnected
	case sf::Socket::Disconnected:
		break;
		// Error
	case sf::Socket::Error:
		break;
		// Default
	default:
		std::cout << "Something broke in switch Server::socket_status()" << std::endl;
	}

	std::string type = socketTransferType == 'r' ? "recieve " : "send ";	// print socket status for current function
	//std::cout << "Socket " << type << *m_socket_msg.at(status) << std::endl;
}


// tick rate
void Client::check_update_time(const float tickRate, const float frameRate)
{
	m_do_tick = (m_current_t - m_last_t_tick) > tickRate ? true : false;		// tick rate
	m_do_frame = (m_current_t - m_last_t_frame) > frameRate ? true : false;		// frame rate
}


// update
void Client::run()
{
	m_clock->restart();
	m_last_t = m_clock->getElapsedTime().asSeconds();

	while (m_render_win->isOpen())
	{
		m_current_t = m_clock->getElapsedTime().asSeconds();
		m_delta_t += m_current_t - m_last_t;

		sf::Event event;
		while (m_render_win->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_render_win->close();
		}



		// input
		m_player_local->dir_input(m_delta_t);
		// shoot TEST
		m_player_local->shoot_input(*m_render_win);	// render win to get mouse pos



		// get if/what to update
		check_update_time(m_tickrate, m_framerate);

		// update network
		//send_packet();
		
		/*if (m_do_tick)
		{
			send_packet();
			receive_packet();

			m_last_t_tick = m_current_t;
		}*/

		send_packet();
		receive_packet();

		// update render
		if (m_do_frame)
		{
			m_render_win->clear(sf::Color::Cyan);

			m_player_server->update();
			m_player_local->update(m_delta_t, *m_render_win);

			m_player_server->render(*m_render_win);
			m_player_local->render(*m_render_win);

			m_render_win->display();

			m_delta_t = 0;
			m_last_t_frame = m_current_t;
		}

		m_last_t = m_clock->getElapsedTime().asSeconds();
	}
}