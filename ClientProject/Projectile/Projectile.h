#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#define SAFE_DEL(x) delete x; x = nullptr;

// forward
class Player;
class Client;
// const
const float PROJECTILE_MAX_SPEED = 50000;


class Projectile
{
public:
	Projectile(const sf::Vector2f& spawnPos, const sf::Vector2f& dir, const sf::Vector2f& size, const sf::Color& color);
	~Projectile();

	const void			bounce(sf::RenderWindow& rWin);
	const bool			destroy_self();

	sf::RectangleShape* get_rect();
	sf::Vector2f*		get_dir();

	void				update(const float deltaT, sf::RenderWindow& rWin);
	void				render(sf::RenderWindow& rWin);

private:
	sf::RectangleShape*	m_rect_shape;

	sf::Clock*			m_clock;

	sf::Vector2f*		m_rect_size;
	sf::Vector2f*		m_dir;
	sf::Vector2f*		m_pos;

	float				m_speed,
						m_max_life;

	size_t				m_bounces,
						m_max_bounces;

	bool				m_destroy_self;
};