#include "ClientPlayerPacket.h"


// cTor
ClientPlayerPacket::ClientPlayerPacket()
	: 
	m_is_connected_this(true),
	m_is_connected_other(false),
	m_current_func(3),
	m_input_x(0),
	m_input_y(0),
	m_input_m_1(0),
	m_new_bullet_this(0),
	m_new_bullet_other(0),
	m_delta_t(0),
	m_player_pos_this(-100, -100),	// init spawn outside screen until connection with server is established, server delivers which side (left-right) this player spawns
	m_player_pos_other(-100, -100),
	m_bullet_dir_this(0, 0),
	m_bullet_dir_other(0, 0),
	m_mouse_pos_this(0, 0),
	m_health_this(100),
	m_health_other(100)
{}


// ostream
std::ostream& operator<<(std::ostream& out, const ClientPlayerPacket& cpp)
{
	return out << "This: " << '(' <<
						cpp.m_player_pos_this.x << ", " <<
						cpp.m_player_pos_this.y << "), Health: " <<
						cpp.m_health_this << "\n"
				<< "Other: " << '(' <<
						cpp.m_player_pos_other.x << ", " <<
						cpp.m_player_pos_other.y << "), Health: " <<
						cpp.m_health_other << "\n" << std::endl;
}


// send packet
sf::Packet& operator<<(sf::Packet& p, const ClientPlayerPacket& cpp)
{
	return p << cpp.m_is_connected_this << cpp.m_input_x << cpp.m_input_y <<
				cpp.m_input_m_1 << cpp.m_mouse_pos_this.x << cpp.m_mouse_pos_this.y <<
				cpp.m_delta_t << cpp.m_health_this << cpp.m_health_other;
}


// receive packet
sf::Packet& operator>>(sf::Packet& p, ClientPlayerPacket& cpp)
{
	return p >> cpp.m_current_func >>
				cpp.m_player_pos_this.x >> cpp.m_player_pos_this.y >> cpp.m_health_this >>
				cpp.m_new_bullet_this >> cpp.m_bullet_dir_this.x >> cpp.m_bullet_dir_this.y >>
				cpp.m_is_connected_other >> cpp.m_player_pos_other.x >> cpp.m_player_pos_other.y >> cpp.m_health_other >>
				cpp.m_new_bullet_other >> cpp.m_bullet_dir_other.x >> cpp.m_bullet_dir_other.y;
}
