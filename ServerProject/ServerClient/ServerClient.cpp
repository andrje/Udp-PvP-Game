#include "ServerClient.h"

#include <iostream>	// delete when final

// forward
#include "../ServerPlayerPacket/ServerPlayerPacket.h"


// cTor
ServerClient::ServerClient(const std::string& ip,
							const unsigned short port,
							const size_t clientNr,
							const sf::Vector2f& spawnPosThis,
							const sf::Vector2f& spawnPosOther)
	:
	m_IP(new std::string(ip)),
	m_port(port),
	m_client_nr(clientNr),
	m_spp(new Spp()),
	m_packet(new sf::Packet())
{
	m_spp->m_player_pos_this = spawnPosThis;
	m_spp->m_player_pos_other = spawnPosOther;
	m_spp->m_health_this = 100;
	m_spp->m_health_other = 100;
}


// dTor
ServerClient::~ServerClient()
{
	SAFE_DEL(m_IP);		SAFE_DEL(m_spp);	SAFE_DEL(m_packet);
}


// get IP
std::string* ServerClient::get_IP()
{
	return m_IP;
}


// get port
unsigned short ServerClient::get_port()
{
	return m_port;
}


// get client nr
size_t ServerClient::get_client_nr()
{
	return m_client_nr;
}


// get spp
Spp* ServerClient::get_spp()
{
	return m_spp;
}


// receive packet
void ServerClient::set_packet(sf::Packet& packet)
{
	packet >> *m_spp;
}


// send packet
sf::Packet* ServerClient::get_packet()
{
	m_packet->clear();
	*m_packet << *m_spp;

	return m_packet;
}


// update
void ServerClient::update()
{
	sf::Vector2f tmp_pos;
	tmp_pos.x = m_spp->m_input_x;
	tmp_pos.y = m_spp->m_input_y;

	if (tmp_pos.x != 0 && tmp_pos.y != 0)
		tmp_pos /= std::sqrt(tmp_pos.x * tmp_pos.x + tmp_pos.y * tmp_pos.y);

	m_spp->m_player_pos_this += tmp_pos * SPEED_BASE * m_spp->m_delta_t;
}