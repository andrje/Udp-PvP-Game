#pragma once

#include "../PlayerSuper/PlayerSuper.h"


class PlayerServer : public PlayerSuper
{
public:
	PlayerServer();

	void	update_projectiles(sf::RenderWindow& rWin, const float deltaT);
	void	update(sf::RenderWindow& rWin, const float deltaT);
};