#pragma once

#include <string>
#include <map>

#include "SFML/System/Vector2.hpp"
#include "SFML/Network/UdpSocket.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

// forward
struct ServerPlayerPacket;
// typedef
using Spp = ServerPlayerPacket;
// const
const float SPEED_BASE = 350;
const float SPEED_DASH = SPEED_BASE * 1.5;
const float SPEED_MAX = SPEED_BASE * SPEED_DASH;


class ServerClient
{
public:
	ServerClient(const std::string& ip,
				const unsigned short port,
				const size_t clientNr,
				const sf::Vector2f& spawnPosThis,
				const sf::Vector2f& spawnPosOther);
	~ServerClient();

	std::string*	get_IP();
	unsigned short	get_port();
	size_t			get_client_nr();
	Spp*			get_spp();

	sf::Packet*		get_packet();
	void			set_packet(sf::Packet& packet);

	void			update();

private:
	std::string*	m_IP;
	unsigned short	m_port;

	size_t			m_client_nr;

	Spp*			m_spp;
	sf::Packet*		m_packet;
};