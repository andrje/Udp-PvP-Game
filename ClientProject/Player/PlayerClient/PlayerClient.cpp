#include <cmath>

#include "PlayerClient.h"

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "../../Projectile/Projectile.h"
// forward
#include "../../Client/Client.h"


// cTor
PlayerClient::PlayerClient()
	:
	m_is_shooting(false),
	m_current_t(0),
	m_last_t(0)
{
	init_shape(30, 4, sf::Color::Magenta);

	for (size_t i = 0; i < 3; i++)
		m_input_vec.push_back(0);
}


// input
void PlayerClient::get_player_input(const float deltaT, sf::RenderWindow& rWin)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_input_vec.at(Input::HORI) = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_input_vec.at(Input::HORI) = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_input_vec.at(Input::VERT) = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_input_vec.at(Input::VERT) = 1;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_is_shooting)
	{
		m_is_shooting = true;
		m_input_vec.at(Input::MOUSE_1) = 1;
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_is_shooting)
	{
		m_is_shooting = false;
	}

	update_packet_input(m_input_vec, rWin, deltaT);
}


// reset
void PlayerClient::reset_player_input()
{
	for (size_t i = 0; i < 3; i++)
		m_input_vec.at(i) = 0;
}


// shoot
//void PlayerClient::shoot_input(sf::RenderWindow& rWin)
//{
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_is_shooting)
//	{
//		m_is_shooting = true;
//		sf::Vector2i mouse_pos = sf::Mouse::getPosition(rWin);
//
//		Projectile* projectile = new Projectile(get_cpp_server_pos_this(),
//												sf::Vector2f(mouse_pos.x, mouse_pos.y),
//												sf::Vector2f(10, 10));
//
//		m_projectiles_vec.push_back(projectile);
//	}
//	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_is_shooting)
//	{
//		m_is_shooting = false;
//	}
//}


// update
void PlayerClient::update(const float deltaT, sf::RenderWindow& rWin)
{
	set_shape_pos(get_cpp_server_pos_this());
	set_health(get_cpp_server_health_this());



	// projectiles
	//for (size_t i = 0; i < get_projectiles_vec().size();)
	//{
	//	get_projectiles_vec().at(i)->update(deltaT, rWin);
	//
	//	// destroy
	//	if (get_projectiles_vec().at(i)->destroy_self())
	//	{
	//		SAFE_DEL(get_projectiles_vec().at(i));
	//		get_projectiles_vec().erase(get_projectiles_vec().begin());
	//		continue;
	//	}

	//	i++;
	//}
}

