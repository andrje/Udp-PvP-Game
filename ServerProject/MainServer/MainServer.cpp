#include "../Server/Server.h"

#include "SFML/Network.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <iostream>


struct PacketData
{
	float x1 = 1, y1 = 2, z1 = 3;
	float x2 = 0, y2 = 0, z2 = 0;
};



void socket_send(sf::Socket::Status& status, sf::Packet& packet)
{
	switch (status)
	{
	case sf::Socket::Done:
		std::cout << "Send done" << '\n' << std::endl;
		packet.clear();
		break;

	case sf::Socket::NotReady:
		std::cout << "Send not ready" << '\n' << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "Socket disconnected" << '\n' << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "Send error" << '\n' << std::endl;
		break;
	}
}

void socket_receive(sf::Socket::Status& status, sf::Packet& packet, PacketData& pd)
{
	switch (status)
	{
	case sf::Socket::Done:
		std::cout << "Receive done" << '\n' << std::endl;
		packet >> pd.x2 >> pd.y2 >> pd.z2;
		break;

	case sf::Socket::NotReady:
		std::cout << "Receive not ready" << '\n' << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "Socket disconnected" << '\n' << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "Receive error" << '\n' << std::endl;
		break;
	}
}


int main()
{
	/*Server server;
	server.run();*/

	sf::UdpSocket socket;
	socket.bind(50001);
	socket.setBlocking(false);
	sf::IpAddress remote_ip;
	unsigned short remote_port;

	sf::Socket::Status status;
	sf::Packet p1, p2;
	PacketData pd;

	bool isPressed = false;
	sf::Clock clock;
	clock.restart();
	float current_t = 0, last_t = 0, tick_r = 0.5;

	while (true)
	{
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isPressed)
		{
			isPressed = true;
			p1 << pd.x1 << pd.y1 << pd.z1;
			status = socket.send(p1, "127.0.0.1", 50001);

			pd.x1 += 3;
			pd.y1 += 3;
			pd.z1 += 3;
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isPressed)
		{
			isPressed = false;
		}

		current_t = clock.getElapsedTime().asSeconds();
		if (current_t - last_t > tick_r)
		{
			socket_send(status, p1);

			status = socket.receive(p2, remote_ip, remote_port);
			socket_receive(status, p2, pd);
			std::cout << pd.x2 << ", " << pd.y2 << ", " << pd.z2 << '\n' << std::endl;

			last_t = current_t;
		}
	}
	


	while (true);
}