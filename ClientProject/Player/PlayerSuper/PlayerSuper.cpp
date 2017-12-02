#include "PlayerSuper.h"

// forward
#include "../../Projectile/Projectile.h"
#include "../../ClientPlayerPacket/ClientPlayerPacket.h"


// static init
Cpp* PlayerSuper::m_cpp = new Cpp();


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


// get projectiles vec
ProjectilesVec& PlayerSuper::get_projectiles_vec()
{
	return m_projectiles_vec;
}


// update packet input
void PlayerSuper::update_packet_input(std::vector<int>& input, const float deltaT)
{
	m_cpp->m_input_x = input.at(Input::HORI);
	m_cpp->m_input_y = input.at(Input::VERT);
	m_cpp->m_input_m_1 = input.at(Input::MOUSE_1);

	m_cpp->m_delta_t = deltaT;
}


// get cpp pos this
sf::Vector2f PlayerSuper::get_cpp_server_pos_this()
{
	return m_cpp->m_player_pos_this;
}


// get cpp pos other
sf::Vector2f PlayerSuper::get_cpp_server_pos_other()
{
	return m_cpp->m_player_pos_other;
}


// get cpp health this
float PlayerSuper::get_cpp_server_health_this()
{
	return m_cpp->m_health_this;
}


// get cpp health other
float PlayerSuper::get_cpp_server_health_other()
{
	return m_cpp->m_health_other;
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

	//std::cout << *m_cpp << '\n' << std::endl;
}


// render
void PlayerSuper::render(sf::RenderWindow& rWin)
{
	rWin.draw(*m_shape);

	if (m_projectiles_vec.size() > 0)
	{
		for (auto& pro : m_projectiles_vec)
			rWin.draw(*pro->get_rect());
	}
}
