#include <iostream>

#include "Server.h"

// forward
#include "../ServerClient/ServerClient.h"
#include "../ServerPlayerPacket/ServerPlayerPacket.h"


// cTor
Server::Server(const std::string & serverIP, const unsigned short serverPort)
	:
	m_socket(new sf::UdpSocket()),
	m_packet(new sf::Packet()),
	m_server_IP(new std::string(serverIP)),
	m_sender_IP(new sf::IpAddress("NoIP")),
	m_server_port(serverPort),
	m_sender_port(0),
	m_nr_clients_connected(0),
	m_clock(new sf::Clock())
{
	m_socket->bind(m_server_port);
	m_socket->setBlocking(false);
}


// dTor
Server::~Server()
{
	SAFE_DEL(m_socket);			SAFE_DEL(m_packet);		SAFE_DEL(m_server_IP);
	SAFE_DEL(m_sender_IP);		SAFE_DEL(m_clock);

	auto& itr = m_client_map.begin();
	if (itr != m_client_map.end())
	{
		SAFE_DEL(itr->second);
		m_client_map.erase(itr);
	}
}


// recieve packets
void Server::receive_packet()
{
	m_packet->clear();
	while(m_socket->receive(*m_packet, *m_sender_IP, m_sender_port) != sf::Socket::Done) {}

	for (auto& itr : m_client_map)
	{
		if (m_sender_IP->toString() == *itr.second->get_IP() &&
			m_sender_port == itr.second->get_port())
		{
			itr.second->set_packet(*m_packet);
		}
	}
}


// update packets
void Server::update_packet()
{
	for (auto& itr : m_client_map)	// update pos, health etc etc
		itr.second->update();

	for (auto& itr_this : m_client_map)	// copy P1's P1 data to P2's P1 data, and vice versa
	{
		for (auto& itr_other : m_client_map)
		{
			if (itr_this.second->get_client_nr() !=
				itr_other.second->get_client_nr())
			{
				itr_other.second->get_spp()->m_is_connected_other =
					itr_this.second->get_spp()->m_is_connected_this;

				itr_other.second->get_spp()->m_player_pos_other =
					itr_this.second->get_spp()->m_player_pos_this;

				itr_other.second->get_spp()->m_health_other = 
					itr_this.second->get_spp()->m_health_this;

				itr_other.second->get_spp()->m_new_bullet_other =
					itr_this.second->get_spp()->m_new_bullet_this;

				itr_other.second->get_spp()->m_bullet_dir_other =
					itr_this.second->get_spp()->m_bullet_dir_this;
			}
		}
	}
}


// send packets
void Server::send_packet()
{
	for (auto& itr : m_client_map)
	{
		m_socket->send(*itr.second->get_packet(), *itr.second->get_IP(), itr.second->get_port());
		itr.second->get_spp()->m_new_bullet_this = 0;	// because im bad at programming
		itr.second->get_spp()->m_new_bullet_other = 0;
	}
}


// inti connect
void Server::init_connect()
{
	std::cout << "Waiting for clients to connect...\n" << std::endl;

	sf::Packet packet;
	sf::IpAddress sender_IP;
	unsigned short sender_port;
	sf::Vector2f spawn_pos_1, spawn_pos_2;
	sf::Vector2f spawn_1(640 / 4, 360 / 2), spawn_2(640 - (640 / 4), 360 / 2);

	do
	{
		while (m_socket->receive(packet, sender_IP, sender_port) != sf::Socket::Done) {} // receive

		if (m_client_map.find(sender_port) == m_client_map.end())
		{
			m_nr_clients_connected++;
			if (m_nr_clients_connected < 2)
			{
				spawn_pos_1 = spawn_1;
				spawn_pos_2 = spawn_2;
			}
			else
			{
				spawn_pos_1 = spawn_2;
				spawn_pos_2 = spawn_1;
			}

			m_client_map.insert(std::make_pair(sender_port,
												new ServerClient(sender_IP.toString(),
																sender_port,
																m_nr_clients_connected,
																spawn_pos_1,
																spawn_pos_2)));

			m_socket->send(*m_client_map[sender_port]->get_packet(),	// send init packet
										*m_client_map[sender_port]->get_IP(),
										m_client_map[sender_port]->get_port());

			std::cout << "Client " << m_nr_clients_connected << " connected" <<
						"\nIP: " << *m_client_map.find(sender_port)->second->get_IP() <<
						"\nPort: " << m_client_map.find(sender_port)->second->get_port() <<
						'\n' << std::endl;
		}
	} while (m_client_map.size() < 2);

	std::cout << "Player connections established" << std::endl;

	// TESTING
	for (auto& client : m_client_map)
		client.second->set_client_state(ClientState::START);

	send_packet();
}


// inti game
void Server::init_game()
{
	std::cout << "Completing start game countdown" << std::endl;

	int count = 3;
	std::string message = "3";
	m_clock->restart();

	do
	{
		if (m_clock->getElapsedTime().asSeconds() > 1)	// count down game start in seconds
		{
			m_clock->restart();
			message = std::to_string(count);
			--count;

			for (auto& client : m_client_map)
				m_socket->send(message.c_str(), sizeof(message), *client.second->get_IP(), client.second->get_port());
		}
	} while (count > -1);

	for (auto& client : m_client_map)
		client.second->set_client_state(ClientState::GAME);

	send_packet();
}


// run connect
void Server::run_game()
{
	std::cout << "Countdown completed. Game started\n" << std::endl;

	while (true)	// fix this to something useful
	{
		receive_packet();
		update_packet();
		send_packet();
	}
}


// run
void Server::run_server()
{
	init_connect();
	init_game();
	run_game();
}
