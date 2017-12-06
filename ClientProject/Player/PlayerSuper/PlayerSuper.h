#pragma once

#include <string>
#include <vector>
#include <iostream>	// delete when final

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Network/UdpSocket.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

// forward
class Projectile;
struct ClientPlayerPacket;
// typedef
using ProjectilesVec = std::vector<Projectile*>;
using Cpp = ClientPlayerPacket;
// const
const float SPEED_BASE = 350;
const float SPEED_DASH = SPEED_BASE * 1.5;
const float SPEED_MAX = SPEED_BASE * SPEED_DASH;


class PlayerSuper
{
public:
	PlayerSuper();
	~PlayerSuper();

	void				init_shape(const float radius, const size_t pointCount, const sf::Color& color);

	void				set_shape_pos(const sf::Vector2f& newPos);
	void				set_health(const float newHealth);
	ProjectilesVec&		get_projectiles_vec();
	void				update_packet_input(std::vector<int>& input, const float deltaT);

	sf::Vector2f		get_cpp_server_pos_this();
	sf::Vector2f		get_cpp_server_pos_other();
	float				get_cpp_server_health_this();
	float				get_cpp_server_health_other();

	sf::Packet*			get_packet();
	void				set_packet(sf::Packet& packet);
	void				set_is_connected(const bool trueFalse);

	void				render(sf::RenderWindow& rWin);

protected:
	enum Input { HORI, VERT, MOUSE_1 };
	
protected:
	sf::CircleShape*	m_shape;
	sf::Packet*			m_packet;

	ProjectilesVec		m_projectiles_vec;
	static Cpp*			m_cpp;
};