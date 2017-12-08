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


// handle bullets
void PlayerClient::update_projectiles(sf::RenderWindow & rWin, const float deltaT)
{
	if (m_new_bullet_this)	// spawn new bullet
	{
		Projectile* projectile = new Projectile(get_cpp_pos_this(), get_cpp_bullet_dir_this(), sf::Vector2f(10, 10));
		m_bullet_vec.push_back(projectile);

		m_new_bullet_this = false;
	}

	for (size_t i = 0; i < m_bullet_vec.size();)
	{
		m_bullet_vec.at(i)->update(deltaT, rWin);	// update 

		if (m_bullet_vec.at(i)->destroy_self())	// destroy
		{
			SAFE_DEL(m_bullet_vec.at(i));
			m_bullet_vec.erase(m_bullet_vec.begin());
			continue;
		}
		i++;
	}
}


// update
void PlayerClient::update(sf::RenderWindow& rWin, const float deltaT)
{
	set_shape_pos(get_cpp_pos_this());
	set_health(get_cpp_health_this());

	update_projectiles(rWin, deltaT);
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