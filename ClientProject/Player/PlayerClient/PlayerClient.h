#pragma once

#include "../PlayerSuper/PlayerSuper.h"

// forward
class Client;


class PlayerClient : public PlayerSuper
{
public:
	PlayerClient();

	void		dir_input(const float deltaT);
	void		shoot_input(sf::RenderWindow& rWin);


	/*void		update(const float deltaT,
						sf::UdpSocket& socket,
						const std::string& serverIP,
						const unsigned short serverPort´,
						sf::RenderWindow& rWin);*/

	void		update(const float deltaT, sf::RenderWindow& rWin);

private:
	std::vector<int>	m_input;

	float				delta_t,
						m_current_t,
						m_last_t;

	bool				m_is_shooting;
};