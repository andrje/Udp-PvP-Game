#pragma once

#include <ostream>

#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"


// client side struct // difference from server side are overloads
struct ClientPlayerPacket
{
	ClientPlayerPacket();

	bool			m_is_connected_this,	// false = disconnected, true = connected
					m_is_connected_other;

	int				m_current_func,
					m_input_x,
					m_input_y,
					m_input_m_1,
					m_bullet_id_this,
					m_bullet_id_other;

	sf::Vector2f	m_player_pos_this,
					m_player_pos_other,
					m_bullet_dir_this,
					m_bullet_dir_other;

	sf::Vector2i	m_mouse_pos_this;

	float			m_delta_t,
					m_health_this,
					m_health_other;

	friend std::ostream& operator<<(std::ostream& out, const ClientPlayerPacket& cpp);
};


// client side send/receive packet overload
sf::Packet& operator <<(sf::Packet& p, const ClientPlayerPacket& cpp);
sf::Packet& operator >>(sf::Packet& p, ClientPlayerPacket& cpp);