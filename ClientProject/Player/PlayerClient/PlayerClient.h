#pragma once

#include "../PlayerSuper/PlayerSuper.h"

class Client;


class PlayerClient : public PlayerSuper
{
public:
	PlayerClient();

	void		move(const float deltaT, sf::RenderWindow& rWin);
	void		shoot(sf::RenderWindow& rWin);


	void		update(const float deltaT,
						sf::UdpSocket& socket,
						const std::string& serverIP,
						const unsigned short serverPort´,
						sf::RenderWindow& rWin);

private:
	bool				m_is_shooting;
};