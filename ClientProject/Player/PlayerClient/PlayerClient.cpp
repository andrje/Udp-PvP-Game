#include <cmath>

#include "PlayerClient.h"

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "../../Projectile/Projectile.h"
#include "../../Client/Client.h"


// cTor
PlayerClient::PlayerClient()
	: m_is_shooting(false)
{
	init_shape(30, 4, sf::Color::Magenta);
}


// input
void PlayerClient::move(const float deltaT, sf::RenderWindow& rWin)
{
	sf::Vector2f tmp_pos(0, 0);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		tmp_pos.x = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		tmp_pos.x = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		tmp_pos.y = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		tmp_pos.y = 1;

	if (tmp_pos.x != 0 && tmp_pos.y != 0)
		tmp_pos /= std::sqrt(tmp_pos.x * tmp_pos.x +
							tmp_pos.y * tmp_pos.y);

	tmp_pos *= SPEED_BASE * deltaT;
	update_packet_pos(tmp_pos);
}


// shoot
void PlayerClient::shoot(sf::RenderWindow& rWin)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_is_shooting)
	{
		//m_is_shooting = true;

		sf::Vector2i mouse_pos = sf::Mouse::getPosition(rWin);

		Projectile* projectile = new Projectile(get_cpp_server_pos_this(),
												sf::Vector2f(mouse_pos.x, mouse_pos.y),
												sf::Vector2f(10, 10));

		m_projectiles_vec.push_back(projectile);
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_is_shooting)
	{
		m_is_shooting = false;
	}
}


// update
void PlayerClient::update(const float deltaT,
							sf::UdpSocket& socket,
							const std::string& serverIP,
							const unsigned short serverPort,
							sf::RenderWindow& rWin)
{
	// input
	move(deltaT, rWin);
	// shoot TEST
	shoot(rWin);

	// network
	send_packet(socket, serverIP, serverPort);
	receive_packet(socket);

	set_shape_pos(get_cpp_server_pos_this());
	set_health(get_cpp_server_health_this());

	// projectiles
	for (size_t i = 0; i < get_projectiles_vec().size();)
	{
		get_projectiles_vec().at(i)->update(deltaT, rWin);

		// destroy
		if (get_projectiles_vec().at(i)->destroy_self())
		{
			SAFE_DEL(get_projectiles_vec().at(i));
			get_projectiles_vec().erase(get_projectiles_vec().begin());
			continue;
		}
		i++;
	}
}
