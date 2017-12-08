#include <iostream>	// delete when final

#include "ServerClient.h"

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


// send packet
sf::Packet* ServerClient::get_packet()
{
	m_packet->clear();
	*m_packet << *m_spp;

	//std::cout << *m_spp << '\n' << std::endl;

	return m_packet;
}


// receive packet
void ServerClient::set_packet(sf::Packet& packet)
{
	packet >> *m_spp;
	
	/*if(m_spp->m_input_m_1 == 1)
		std::cout << m_spp->m_input_m_1 << '\n' << std::endl;*/
}


// set current func
void ServerClient::set_client_state(const std::size_t func)
{
	m_spp->m_current_func = func;
}


// get is connected
bool ServerClient::get_is_connected()
{
	return m_spp->m_is_connected_this;
}


// update player pos
void ServerClient::update_player_pos()
{
	sf::Vector2f tmp_input;
	tmp_input.x = m_spp->m_input_x;
	tmp_input.y = m_spp->m_input_y;

	if (tmp_input.x != 0 && tmp_input.y != 0)
		tmp_input /= std::sqrt(tmp_input.x * tmp_input.x + tmp_input.y * tmp_input.y);

	m_spp->m_player_pos_this += tmp_input * SPEED_BASE * m_spp->m_delta_t;
}


// update bullet dir
void ServerClient::update_bullet()
{
	if (m_spp->m_input_m_1 == 1)
	{
		m_spp->m_new_bullet_this = m_spp->m_input_m_1;
		m_spp->m_input_m_1 = 0;
	}

	if (m_spp->m_new_bullet_this == 1)
	{
		sf::Vector2f tmp_mouse;	// because of sf::mouse is vector2i, and has no overload for vector2f
		tmp_mouse.x = m_spp->m_mouse_pos_this.x;
		tmp_mouse.y = m_spp->m_mouse_pos_this.y;

		sf::Vector2f tmp_dir = tmp_mouse - m_spp->m_player_pos_this;

		if (tmp_dir.x != 0 && tmp_dir.y != 0)
			tmp_dir /= std::sqrt(tmp_dir.x * tmp_dir.x + tmp_dir.y * tmp_dir.y);

		m_spp->m_bullet_dir_this = tmp_dir;
	}
}


// update
void ServerClient::update()
{
	update_player_pos();
	update_bullet();
}