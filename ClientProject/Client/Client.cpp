#include <iostream>
#include <string>

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
	m_event(new sf::Event()),
	m_tickrate(0.0167),		// 30 = 0.0333 | 60 = 0.0167 | 120 = 0.0083
	m_framerate(0.0083),
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
										"Skjut som fan",
										sf::Style::Default,
										AA);

	m_player_local = new PlayerClient();
	m_player_server = new PlayerServer();

	m_render_win->clear(sf::Color(30, 30, 30));
	m_player_server->render(*m_render_win);
	m_render_win->display();

	std::function<void(Client&)> init_func_ptr = &Client::start;
	m_funcptr_vec.push_back(init_func_ptr);
	init_func_ptr = &Client::game;
	m_funcptr_vec.push_back(init_func_ptr);
	init_func_ptr = &Client::end;
	m_funcptr_vec.push_back(init_func_ptr);
	init_func_ptr = &Client::idle;
	m_funcptr_vec.push_back(init_func_ptr);
}


// dTor
Client::~Client()
{
	m_player_local->set_is_connected(false);

	while (m_socket->send(*m_player_local->get_packet(), *m_server_IP, m_server_port) != sf::Socket::Done);
}


// init connect
void Client::init_connect()
{
	char buffer[1024];
	size_t received = 0;
	sf::IpAddress sender_IP;
	unsigned short sender_port;
	m_packet->clear();

	std::cout << "Waiting for server response..." << std::endl;

	bool has_received = false;
	size_t timed_out = 20;
	m_clock->restart();

	do
	{
		m_socket->send(*m_player_local->get_packet(), *m_server_IP, m_server_port);

		if (m_socket->receive(*m_packet, sender_IP, sender_port) == sf::Socket::Done)
		{
			m_player_local->set_packet(*m_packet);
			has_received = true;
		}
	}
	while (m_clock->getElapsedTime().asSeconds() < timed_out && !has_received);

	std::string status = has_received ? "Connected to server. Waiting for other player..." : "Server connection timed out. Put another quarter in and try again..";
	std::cout << status << std::endl;
}


// send packet
void Client::send_packet()
{
	if (m_do_tick)	// tickrate
	{
		if (m_socket->send(*m_player_local->get_packet(), *m_server_IP, m_server_port) == sf::Socket::Done)
			m_player_local->reset_player_input();	// an atempt at not missing player inputs without sf::Event, not so great result
	}
}


// receive packet
void Client::receive_packet()
{
	sf::IpAddress sender_IP;
	unsigned short sender_port;

	m_packet->clear();
	m_socket->receive(*m_packet, sender_IP, sender_port);

	m_player_local->set_packet(*m_packet);
}


// tick rate
void Client::check_update_time(const float tickRate, const float frameRate)
{
	m_do_tick = (m_first_t - m_last_t_tick) > tickRate ? true : false;		// tick rate
	m_do_frame = (m_first_t - m_last_t_frame) > frameRate ? true : false;	// frame rate
}


// start
void Client::start()
{
	std::cout << "Both players connected.\n\nGame starts in... ";

	char buffer[1024];
	size_t received = 0;
	sf::IpAddress sender_IP;
	unsigned short sender_port;

	int count_down;

	do
	{
		while (m_socket->receive(buffer, sizeof(buffer), received, sender_IP, sender_port) != sf::Socket::Done) {}
		count_down = (int)*buffer - '0';

		if(count_down > 0)
			std::cout << count_down << "... ";

	} while (count_down > 0);

	std::cout << "It's on!\n" << std::endl;
}


// game
void Client::game()
{
	// update player/frame
	if (m_do_frame)
	{
		m_render_win->clear(sf::Color(32, 32, 32));

		m_player_server->update(*m_render_win, m_delta_t);	// update oponent
		m_player_local->update(*m_render_win, m_delta_t);	// update this player

		m_player_server->render(*m_render_win);	// render
		m_player_local->render(*m_render_win);

		m_render_win->display();

		m_delta_t = 0;
		m_last_t_frame = m_first_t;
	}

	// get player input
	m_player_local->get_player_input(m_delta_t, *m_render_win);	// render win for mouse pos
}


// end
void Client::end()
{
	std::cout << "end" << std::endl;
}


// idle
void Client::idle() {}


// event handler
void Client::event_handler()
{
	while (m_render_win->pollEvent(*m_event))
	{
		if (m_event->type == sf::Event::Closed)
			m_render_win->close();
	}
}


// run client
void Client::run_client()
{
	m_clock->restart();
	m_last_t = m_clock->getElapsedTime().asSeconds();

	init_connect();

	while (m_render_win->isOpen())
	{
		m_first_t = m_clock->getElapsedTime().asSeconds();
		m_delta_t += m_first_t - m_last_t;

		// sf::Event
		event_handler();

		// get if/what to update
		check_update_time(m_tickrate, m_framerate);

		// run current state
		m_funcptr_vec[m_player_local->get_current_state()](*this);

		// update network
		send_packet();
		receive_packet();

		m_last_t = m_clock->getElapsedTime().asSeconds();
	}
}





















// packet_status
void Client::packet_status(const char socketTransferType, sf::Socket::Status& status)
{
	switch (status)
	{
		// Done
	case sf::Socket::Done:

		/*if (socketTransferType == 'r')
		m_player_local->set_packet(*m_packet);*/

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


//while (m_render_win->isOpen())
//{
//	m_first_t = m_clock->getElapsedTime().asSeconds();
//	m_delta_t += m_first_t - m_last_t;

//	sf::Event event;
//	while (m_render_win->pollEvent(event))
//	{
//		if (event.type == sf::Event::Closed)
//			m_render_win->close();
//	}

//	
//	// input
//	m_player_local->dir_input(m_delta_t);
//	// shoot TEST
//	//m_player_local->shoot_input(*m_render_win);	// render win to get mouse pos



//	// get if/what to update
//	check_update_time(m_tickrate, m_framerate);

//	// update network
//	send_packet();
//	receive_packet();

//	// update render
//	if (m_do_frame)
//	{
//		m_render_win->clear(sf::Color::Cyan);

//		m_player_server->update();
//		m_player_local->update(m_delta_t, *m_render_win);

//		m_player_server->render(*m_render_win);
//		m_player_local->render(*m_render_win);

//		m_render_win->display();

//		m_delta_t = 0;
//		m_last_t_frame = m_first_t;
//	}

//	m_last_t = m_clock->getElapsedTime().asSeconds();

//	//std::cout << "main" << std::endl;
//}