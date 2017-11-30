#pragma once

#include <vector>
#include <ostream>

#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"


// client side struct // difference from server side are overloads
struct ClientPlayerPacket
{
	ClientPlayerPacket();

	sf::Vector2f	m_player_pos_this,
					m_player_pos_other;
	float			m_health_this,
					m_health_other;

	//std::vector<Bullets*>	m_v_bullets;

	friend std::ostream& operator<<(std::ostream& out, const ClientPlayerPacket& cpp);
};


// client side send/receive packet overload
sf::Packet& operator <<(sf::Packet& p, const ClientPlayerPacket& cpp);
sf::Packet& operator >>(sf::Packet& p, ClientPlayerPacket& cpp);