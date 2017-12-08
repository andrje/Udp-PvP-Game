#pragma once

#include <string>
#include <map>

#include "SFML/System/Vector2.hpp"
#include "SFML/Network/UdpSocket.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

// forward
struct ServerPlayerPacket;
// typedef
using Spp = ServerPlayerPacket;
// const
const float SPEED_BASE = 1000;


class ServerClient
{
public:
	ServerClient(const std::string& ip,
				const unsigned short port,
				const size_t clientNr,
				const sf::Vector2f& spawnPosThis,
				const sf::Vector2f& spawnPosOther);
	~ServerClient();

	std::string*	get_IP();
	unsigned short	get_port();
	size_t			get_client_nr();
	Spp*			get_spp();

	sf::Packet*		get_packet();
	void			set_packet(sf::Packet& packet);
	void			set_client_state(const std::size_t func);
	bool			get_is_connected();

	void			update_player_pos();
	void			update_bullet();
	void			update();

private:
	std::string*	m_IP;
	unsigned short	m_port;

	size_t			m_client_nr,
					m_win_width,
					m_win_height,
					m_player_size;

	Spp*			m_spp;
	sf::Packet*		m_packet;
};