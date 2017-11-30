#include "ClientPlayerPacket.h"


// cTor
ClientPlayerPacket::ClientPlayerPacket()
	: m_player_pos_this(-100, -100),	// init spawn outside screen
	m_player_pos_other(-100, -100),
	m_health_this(100),
	m_health_other(100)
{}


// ostream
std::ostream & operator<<(std::ostream & out, const ClientPlayerPacket & cpp)
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
	return p << cpp.m_player_pos_this.x << cpp.m_player_pos_this.y << cpp.m_health_this;
}


// receive packet
sf::Packet& operator>>(sf::Packet& p, ClientPlayerPacket& cpp)
{
	return p >> cpp.m_player_pos_this.x >> cpp.m_player_pos_this.y >> cpp.m_health_this >>
				cpp.m_player_pos_other.x >> cpp.m_player_pos_other.y >> cpp.m_health_other;
}