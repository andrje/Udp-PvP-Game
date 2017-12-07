#pragma once

#include <string>
#include <vector>
#include <functional>

#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"

// forward
class PlayerClient;
class PlayerServer;
class Client;
// typedef
using FuncPtrVec = std::vector <std::function<void(Client&)>>;


class Client
{
public:
	Client(const std::string& serverIP = "127.0.0.1", const unsigned short serverPort = 50000);
	~Client();

	void		init_connect();
	void		send_packet();
	void		receive_packet();
	void		packet_status(const char socketTransferType, sf::Socket::Status& status);

	void		check_update_time(const float tickRate, const float frameRate);
	void		run();

	void		start();
	void		game();
	void		end();
	void		idle();

	void		event_handler();

private:
	sf::UdpSocket*		m_socket;
	sf::Packet*			m_packet;
	std::string*		m_client_IP;
	std::string*		m_server_IP;
	unsigned short		m_client_port,
						m_server_port;

private:
	sf::RenderWindow*	m_render_win;
	size_t				m_win_width,
						m_win_height;

	sf::Clock*			m_clock;
	sf::Event*			m_event;

	float				m_tickrate,
						m_framerate,
						m_first_t,
						m_last_t,
						m_last_t_tick,
						m_last_t_frame,
						m_delta_t;

	bool				m_do_tick,
						m_do_frame;

private:
	PlayerClient*		m_player_local;
	PlayerServer*		m_player_server;

	FuncPtrVec			m_funcptr_vec;
};
