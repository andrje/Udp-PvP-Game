#include "ServerPlayerPacket.h"


// cTor
ServerPlayerPacket::ServerPlayerPacket()
	: m_player_pos_this(0, 0),
	m_player_pos_other(0, 0),
	m_health_this(0),
	m_health_other(0),
	m_client_IP(new std::string(" ")),
	m_client_port(0)
{
	m_input_x = 0;
	m_input_y = 0;
	m_input_m_1 = 0;
	m_delta_t = 0;
}


// dTor
ServerPlayerPacket::~ServerPlayerPacket()
{
	delete m_client_IP;
	m_client_IP = nullptr;
}


// set player pos other
void ServerPlayerPacket::set_player_pos_other(const sf::Vector2f & newPos)
{
	m_player_pos_other = newPos;
}


// set player health other
void ServerPlayerPacket::set_player_health_other(const float newHealth)
{
	m_health_other = newHealth;
}


// ostream
std::ostream& operator<<(std::ostream& out, const ServerPlayerPacket& cpp)
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
sf::Packet& operator<<(sf::Packet& p, const ServerPlayerPacket& cpp)
{
	return p << cpp.m_player_pos_this.x << cpp.m_player_pos_this.y << cpp.m_health_this <<
				cpp.m_player_pos_other.x << cpp.m_player_pos_other.y << cpp.m_health_other;
}


// receive packet
sf::Packet& operator>>(sf::Packet& p, ServerPlayerPacket& cpp)
{
	//return p >> cpp.m_player_pos_this.x >> cpp.m_player_pos_this.y >> cpp.m_health_this;

	return p >> cpp.m_input_x >> cpp.m_input_y >> cpp.m_input_m_1 >> cpp.m_delta_t << cpp.m_health_this;
}
