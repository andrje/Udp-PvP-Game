#pragma once

#include <vector>

#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"


// server side struct // difference from client side are overloads
struct ServerPlayerPacket
{
public:
	ServerPlayerPacket();
	~ServerPlayerPacket();

public:
	std::string*	m_client_IP;
	unsigned short	m_client_port;

	int				m_input_x, m_input_y, m_input_m_1;

	sf::Vector2f	m_player_pos_this,
					m_player_pos_other;
	float			m_delta_t,
					m_health_this,
					m_health_other;

	//std::vector<Bullets*>	m_v_bullets;

	friend std::ostream& operator<<(std::ostream& out, const ServerPlayerPacket& cpp);
};


// server side send/receive packet overload
sf::Packet& operator <<(sf::Packet& p, const ServerPlayerPacket& cpp);
sf::Packet& operator >>(sf::Packet& p, ServerPlayerPacket& cpp);
