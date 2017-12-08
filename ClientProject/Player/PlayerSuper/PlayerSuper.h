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
using Cpp =			ClientPlayerPacket;
using BulletVec =	std::vector<Projectile*>;
// const
const float SPEED_BASE = 350;


class PlayerSuper
{
public:
	PlayerSuper();
	~PlayerSuper();

	void				init_shape(const float radius, const size_t pointCount, const sf::Color& color);

	void				set_shape_pos(const sf::Vector2f& newPos);
	void				set_health(const float newHealth);

	void				update_packet_input(std::vector<int>& input, sf::RenderWindow& rWin, const float deltaT);

	sf::Vector2f		get_cpp_pos_this();
	sf::Vector2f		get_cpp_pos_other();
	float				get_cpp_health_this();
	float				get_cpp_health_other();
	sf::Vector2f		get_cpp_bullet_dir_this();
	sf::Vector2f		get_cpp_bullet_dir_other();

	sf::Packet*			get_packet();
	void				set_packet(sf::Packet& packet);
	int					get_current_state();
	void				set_is_connected(const bool trueFalse);

	void				render(sf::RenderWindow& rWin);

protected:
	enum Input { HORI, VERT, MOUSE_1 };
	
protected:
	sf::CircleShape*	m_shape;
	sf::Packet*			m_packet;

	static Cpp*			m_cpp;
	BulletVec			m_bullet_vec;

	static bool			m_new_bullet_this,
						m_new_bullet_other;
};
