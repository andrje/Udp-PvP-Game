#pragma once

#include <ostream>

#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"


// server side struct // difference from client side are overloads
struct ServerPlayerPacket
{
	ServerPlayerPacket();
	~ServerPlayerPacket();

	std::string*	m_client_IP;
	unsigned short	m_client_port;

	bool			m_is_connected_this,	// true = connected, false = disconnected
					m_is_connected_other;

	int				m_current_func,
					m_input_x,
					m_input_y,
					m_input_m_1,
					m_new_bullet_this,
					m_new_bullet_other;

	sf::Vector2f	m_player_pos_this,
					m_player_pos_other,
					m_bullet_dir_this,
					m_bullet_dir_other;

	sf::Vector2i	m_mouse_pos_this;

	float			m_delta_t,
					m_health_this,
					m_health_other;
};


// server side send/receive packet overload
sf::Packet& operator <<(sf::Packet& p, const ServerPlayerPacket& cpp);
sf::Packet& operator >>(sf::Packet& p, ServerPlayerPacket& cpp);
