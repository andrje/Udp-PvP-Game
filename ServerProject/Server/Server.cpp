#include <iostream>

#include "Server.h"

// forward
#include "../ServerClient/ServerClient.h"
#include "../ServerPlayerPacket/ServerPlayerPacket.h"	// delete later, for testing


// cTor
Server::Server(const std::string & serverIP,
				const unsigned short serverPort)
	: m_socket(new sf::UdpSocket()),
	m_server_IP(new std::string(serverIP)),
	m_server_port(serverPort)
{
	m_socket->bind(m_server_port);
	m_socket->setBlocking(true);
}


// dTor
Server::~Server()
{
	SAFE_DEL(m_socket);			SAFE_DEL(m_server_IP);

	auto& itr = m_client_map.begin();
	if (itr != m_client_map.end())
	{
		SAFE_DEL(itr->second);
		m_client_map.erase(itr);
	}
}


// inti connect
void Server::init_connect()
{
	std::cout << "Waiting for clients...\n" << std::endl;

	sf::Packet packet;
	sf::IpAddress sender_IP;
	unsigned short sender_port;
	size_t clients = 0;
	sf::Vector2f spawn_1, spawn_2;
	sf::Vector2f spawn_pos_1(640 / 4, 360 / 2), spawn_pos_2(640 - (640 / 4), 360 / 2);

	do
	{
		//std::cout << "receive before" << std::endl;
		m_socket->receive(packet, sender_IP, sender_port);
		//std::cout << "receive after" << '\n' << std::endl;

		if (m_client_map.find(sender_port) == m_client_map.end())
		{
			clients++;
			if (clients <= 1)
			{
				spawn_1 = spawn_pos_1;
				spawn_2 = spawn_pos_2;
			}
			else
			{
				spawn_1 = spawn_pos_2;
				spawn_2 = spawn_pos_1;
			}

			m_client_map.insert(std::make_pair(sender_port,
												new ServerClient(sender_IP.toString(),
																sender_port,
																spawn_1,
																spawn_2)));

			m_client_map[sender_port]->init_packet(*m_socket);

			std::cout << "Client " << clients << " connected." <<
						"\nIP: " << *m_client_map.find(sender_port)->second->get_IP() <<
						"\nPort: " << m_client_map.find(sender_port)->second->get_port() <<
						'\n' << std::endl;
		}

	} while (m_client_map.size() < 2);

	std::cout << "Init done" << std::endl;
}


// recieve packets
void Server::recieve_packets()
{
	sf::Packet packet;
	sf::IpAddress sender_IP;
	std::string matching_IP;
	unsigned short sender_port, matching_port;

	//std::cout << "receive before" << std::endl;
	m_socket->receive(packet, sender_IP, sender_port);
	//std::cout << "receive after" << '\n' << std::endl;

	for (auto& itr_this : m_client_map)
	{
		matching_IP = *itr_this.second->get_IP();
		matching_port = itr_this.second->get_port();

		if (sender_IP.toString() == matching_IP &&
			sender_port	== matching_port)
		{
			itr_this.second->set_packet(packet);
		}
	}
}


// update packets
void Server::update_packets()
{
	std::string this_IP, other_IP;
	unsigned short this_port, other_port;
	sf::Vector2f pos_this;
	float health_this;

	for (auto& itr_this : m_client_map)
	{
		this_IP = *itr_this.second->get_IP();
		this_port = itr_this.second->get_port();

		for (auto& itr_other : m_client_map)
		{
			other_IP = *itr_other.second->get_IP();
			other_port = itr_other.second->get_port();
			
			if (other_port != this_port)
			{
				pos_this = itr_this.second->get_spp()->m_player_pos_this;
				health_this = itr_this.second->get_spp()->m_health_this;

				itr_other.second->get_spp()->m_player_pos_other = pos_this;
				itr_other.second->get_spp()->m_health_other = health_this;
			}
		}
	}
}


// send packets
void Server::send_packets()
{
	sf::Packet packet;
	std::string receiver_IP;
	unsigned short sender_port, receiver_port;

	for (auto& itr_this : m_client_map)
	{
		packet << itr_this.second->get_packet();
		sender_port = itr_this.second->get_port();

		for (auto& itr_other : m_client_map)
		{
			receiver_IP = *itr_other.second->get_IP();
			receiver_port = itr_other.second->get_port();

			if (sender_port != receiver_port)
			{
				//std::cout << "send before" << std::endl;
				m_socket->send(packet, receiver_IP, receiver_port);
				//std::cout << "send after" << '\n' << std::endl;
			}
		}
	}
}


// transfer packets
void Server::update()
{
	std::cout << "Starting game\n" << std::endl;

	while (m_client_map.size() == 2)
	{
		recieve_packets();
		update_packets();
		send_packets();

		//std::cout << "update()" << std::endl;
	}
}


// run
void Server::run()
{
	init_connect();
	update();
}
