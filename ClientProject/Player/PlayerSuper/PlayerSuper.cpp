#include "PlayerSuper.h"

// forward
#include "../../Projectile/Projectile.h"
#include "../../ClientPlayerPacket/ClientPlayerPacket.h"


// static init
Cpp* PlayerSuper::m_cpp = new Cpp();


// cTor
PlayerSuper::PlayerSuper()
	: m_shape(new sf::CircleShape())
{}


// dTor
PlayerSuper::~PlayerSuper()
{
	SAFE_DEL(m_shape);		SAFE_DEL(m_cpp);
	SAFE_DEL(m_cpp);
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


// get shape
sf::CircleShape* PlayerSuper::get_shape()
{
	return m_shape;
}


// get health
float PlayerSuper::get_health()
{
	return m_cpp->m_health_this;
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


// set packet pos
void PlayerSuper::update_packet_pos(const sf::Vector2f& newPos)
{
	m_cpp->m_player_pos_this += newPos;
}


// update packet health
void PlayerSuper::update_packet_health(const float newHealth)
{
	m_cpp->m_health_this = newHealth;
}


// send packet
void PlayerSuper::send_packet(sf::UdpSocket& socket,
								const std::string& serverIP,
								const unsigned short serverPort)
{
	sf::Packet packet;
	packet << *m_cpp;
	//std::cout << "send before" << std::endl;
	socket.send(packet, serverIP, serverPort);
	//std::cout << "send after" << '\n' << std::endl;
}


// receive packet
void PlayerSuper::receive_packet(sf::UdpSocket& socket)
{
	sf::Packet packet;
	sf::IpAddress sender_IP;
	unsigned short sender_port;
	//std::cout << "receive before" << std::endl;
	socket.receive(packet, sender_IP, sender_port);
	//std::cout << "receive after" << '\n' << std::endl;
	packet >> *m_cpp;
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


// print packet
void PlayerSuper::print_packet_struct()
{
	std::cout << *m_cpp << std::endl;
}
