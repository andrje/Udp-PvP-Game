#include <cmath>
#include <vector>

#include "Projectile.h"

// forward
#include "../Client/Client.h"

#include <iostream>	// delete when final


// cTor
Projectile::Projectile(const sf::Vector2f& spawnPos, const sf::Vector2f& dir, const sf::Vector2f& size, const sf::Color& color)
	:
	m_clock(new sf::Clock()),
	m_max_life(45),
	m_bounces(0),
	m_max_bounces(2),
	m_player_size(30),
	m_destroy_self(false),
	m_player_pos(new sf::Vector2f())
{
	m_pos = new sf::Vector2f(spawnPos);
	m_dir = new sf::Vector2f(dir);
	m_rect_size = new sf::Vector2f(size);

	m_rect_shape = new sf::RectangleShape(sf::Vector2f(*m_rect_size));
	m_rect_shape->setOrigin(m_rect_size->x / 2, m_rect_size->y / 2);
	m_rect_shape->setFillColor(color);
	m_rect_shape->setPosition(*m_pos);

	m_player_half = m_player_size / 2;
}


// dTor
Projectile::~Projectile()
{
	SAFE_DEL(m_rect_shape);		SAFE_DEL(m_rect_size);
	SAFE_DEL(m_dir);			SAFE_DEL(m_pos);
	SAFE_DEL(m_clock);
}


// bounce
const void Projectile::bounce(sf::RenderWindow& rWin)
{
	if (m_bounces < m_max_bounces)
	{
		// bounce x
		if (m_rect_shape->getPosition().x > rWin.getSize().x ||
			m_rect_shape->getPosition().x < 0)
		{
			float new_x = get_dir()->x * -1;
			get_dir()->x = new_x;

			m_bounces++;
		}
		// bounce y
		if (m_rect_shape->getPosition().y > rWin.getSize().y ||
			m_rect_shape->getPosition().y < 0)
		{
			float new_y = get_dir()->y * -1;
			get_dir()->y = new_y;

			m_bounces++;
		}
	}
}


// hit detection
const bool Projectile::is_hit()
{
	sf::Vector2u mid(m_player_pos->x, m_player_pos->y);	// range based hit detction
	int upper, downer, righter, lefter;

	upper = mid.y - m_player_half;
	downer = mid.y + m_player_half;
	righter = mid.x + m_player_half;
	lefter = mid.x - m_player_half;

	if (m_pos->y >= upper	&&	m_pos->y <= downer &&
		m_pos->x <= righter &&	m_pos->x >= lefter)
	{
		return true;
	}

	return false;
}


// destroy self
const bool Projectile::destroy_self()
{
	m_destroy_self = m_clock->getElapsedTime().asSeconds() > m_max_life &&
					m_bounces > m_max_bounces - 1
					? true : false;

	return m_destroy_self;
}


// get rect
sf::RectangleShape* Projectile::get_rect()
{
	return m_rect_shape;
}


// get dir
sf::Vector2f* Projectile::get_dir()
{
	return m_dir;
}


// update
void Projectile::update(const float deltaT, sf::RenderWindow& rWin, const sf::Vector2f& playerPos, const std::size_t playerSize)
{
	bounce(rWin);

	*m_pos += *m_dir * PROJECTILE_MAX_SPEED * deltaT;
	m_rect_shape->setPosition(*m_pos);

	*m_player_pos = playerPos;
	is_hit();
}


// draw
void Projectile::render(sf::RenderWindow& rWin)
{
	rWin.draw(*m_rect_shape);
}