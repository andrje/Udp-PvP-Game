#pragma once

#include "../PlayerSuper/PlayerSuper.h"

// forward
class Client;

class PlayerClient : public PlayerSuper
{
public:
	PlayerClient();

	void		get_player_input(const float deltaT, sf::RenderWindow& rWin);
	void		reset_player_input();

	void		update_projectiles(sf::RenderWindow& rWin, const float deltaT);
	void		update(sf::RenderWindow& rWin, const float deltaT);

private:
	std::vector<int>	m_input_vec;

	float				delta_t,
						m_current_t,
						m_last_t;

	bool				m_is_shooting;
};