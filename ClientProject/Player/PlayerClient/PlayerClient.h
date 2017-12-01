#pragma once

#include "../PlayerSuper/PlayerSuper.h"

// forward
class Client;


class PlayerClient : public PlayerSuper
{
public:
	PlayerClient();

	void		move(const float deltaT);
	void		shoot(sf::RenderWindow& rWin);


	void		update(const float deltaT,
						sf::UdpSocket& socket,
						const std::string& serverIP,
						const unsigned short serverPort´,
						sf::RenderWindow& rWin);

private:
	std::vector<int>	m_input;

	bool				m_is_shooting;
};