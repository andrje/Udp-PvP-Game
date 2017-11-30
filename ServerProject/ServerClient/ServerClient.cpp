#include "ServerClient.h"

#include <iostream>	// delete when final

// forward
#include "../ServerPlayerPacket/ServerPlayerPacket.h"
#include "../Server/Server.h"


// cTor
ServerClient::ServerClient(const std::string& ip,
							const unsigned short port,
							const sf::Vector2f& spawnPosThis,
							const sf::Vector2f& spawnPosOther)
	: m_IP(new std::string(ip)),
	m_port(port),
	m_spp(new Spp())
{
	m_spp->m_player_pos_this = spawnPosThis;
	m_spp->m_player_pos_other = spawnPosOther;
	m_spp->m_health_this = 100;
	m_spp->m_health_other = 100;
}


// dTor
ServerClient::~ServerClient()
{
	SAFE_DEL(m_IP);		SAFE_DEL(m_spp);
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


// get spp
Spp* ServerClient::get_spp()
{
	return m_spp;
}


// init packet
void ServerClient::init_packet(sf::UdpSocket& socket)
{
	sf::Packet packet;
	packet << *m_spp;
	//std::cout << "send before" << std::endl;
	socket.send(packet, *m_IP, m_port);
	//std::cout << "send after" << '\n' << std::endl;
}


// set packet
void ServerClient::set_packet(sf::Packet& packet)
{
	packet >> *m_spp;

	//std::cout << *m_spp << std::endl;
}


// get apcket
sf::Packet ServerClient::get_packet()
{
	sf::Packet packet;
	packet << *m_spp;

	//std::cout << *m_spp << std::endl;

	return packet;
}

