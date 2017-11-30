#pragma once

#include <string>
#include <map>

#include "SFML/System/Vector2.hpp"
#include "SFML/Network/UdpSocket.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

struct ServerPlayerPacket;
class ServerClient;

using Spp = ServerPlayerPacket;
using ClientMap = std::map<unsigned short, ServerClient*>;

class ServerClient
{
public:
	ServerClient(const std::string& ip,
				const unsigned short port,
				const sf::Vector2f& spawnPosThis,
				const sf::Vector2f& spawnPosOther);
	~ServerClient();

	std::string*	get_IP();
	unsigned short	get_port();
	Spp*			get_spp();

	void			init_packet(sf::UdpSocket& socket);
	void			set_packet(sf::Packet& packet);
	sf::Packet		get_packet();

private:
	std::string*	m_IP;
	unsigned short	m_port;

	Spp*			m_spp;
};