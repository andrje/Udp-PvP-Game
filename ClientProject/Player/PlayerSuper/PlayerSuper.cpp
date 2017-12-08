#include "PlayerSuper.h"

#include "SFML/Window/Mouse.hpp"
#include "../../Projectile/Projectile.h"

// forward
#include "../../ClientPlayerPacket/ClientPlayerPacket.h"


// static init
Cpp* PlayerSuper::m_cpp = new Cpp();
bool PlayerSuper::m_new_bullet_this = false;
bool PlayerSuper::m_new_bullet_other = false;


// cTor
PlayerSuper::PlayerSuper()
	:
	m_shape(new sf::CircleShape()),
	m_packet(new sf::Packet())
{}


// dTor
PlayerSuper::~PlayerSuper()
{
	SAFE_DEL(m_shape);		SAFE_DEL(m_cpp);	SAFE_DEL(m_packet);
}


// set shape
void PlayerSuper::init_shape(const float radius,
							const size_t pointCount,
							const sf::Color& color)
{
	m_shape->setRadius(radius);
	m_shape->setPointCount(pointCount);
	m_shape->setOrigin(sf::Vector2f(radius, radius));
	m_shape->setFillColor(color);
	m_shape->setPosition(-100, -100);
}


// set shape pos
void PlayerSuper::set_shape_pos(const sf::Vector2f& newPos)
{
	m_shape->setPosition(newPos);
}


// set health
void PlayerSuper::set_health(const float newHealth)
{
	m_cpp->m_health_this = newHealth;
}


// update packet input
void PlayerSuper::update_packet_input(std::vector<int>& input, sf::RenderWindow& rWin, const float deltaT)
{
	m_cpp->m_input_x = input.at(Input::HORI);
	m_cpp->m_input_y = input.at(Input::VERT);
	m_cpp->m_input_m_1 = input.at(Input::MOUSE_1);

	m_cpp->m_mouse_pos_this = sf::Mouse::getPosition(rWin);

	m_cpp->m_delta_t = deltaT;
}


// get cpp pos this
sf::Vector2f PlayerSuper::get_cpp_pos_this()
{
	return m_cpp->m_player_pos_this;
}


// get cpp pos other
sf::Vector2f PlayerSuper::get_cpp_pos_other()
{
	return m_cpp->m_player_pos_other;
}


// get cpp health this
float PlayerSuper::get_cpp_health_this()
{
	return m_cpp->m_health_this;
}


// get cpp health other
float PlayerSuper::get_cpp_health_other()
{
	return m_cpp->m_health_other;
}


// get cpp server bullet dir this
sf::Vector2f PlayerSuper::get_cpp_bullet_dir_this()
{
	return m_cpp->m_bullet_dir_this;
}


// get cpp server bullet dir other
sf::Vector2f PlayerSuper::get_cpp_bullet_dir_other()
{
	return m_cpp->m_bullet_dir_other;
}


// get packet
sf::Packet* PlayerSuper::get_packet()
{
	m_packet->clear();
	*m_packet << *m_cpp;

	return m_packet;
}


// set packet
void PlayerSuper::set_packet(sf::Packet& packet)
{
	packet >> *m_cpp;

	if (m_cpp->m_new_bullet_this == 1)
		m_new_bullet_this = true;

	if (m_cpp->m_new_bullet_other == 1)
		m_new_bullet_other = true;
}


// get current func
int PlayerSuper::get_current_state()
{
	return m_cpp->m_current_func;
}


// set is connected
void PlayerSuper::set_is_connected(const bool trueFalse)
{
	m_cpp->m_is_connected_this = trueFalse;
}


// render
void PlayerSuper::render(sf::RenderWindow& rWin)
{
	rWin.draw(*m_shape);

	if (m_bullet_vec.size() > 0)
	{
		for (auto& pro : m_bullet_vec)
			rWin.draw(*pro->get_rect());
	}
}
