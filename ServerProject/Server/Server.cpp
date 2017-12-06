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
	m_nr_clients_connected(0)
{
	m_socket->bind(m_server_port);
	m_socket->setBlocking(false);

	m_socket_msg.push_back(new std::string("done"));
	m_socket_msg.push_back(new std::string("not ready"));
	m_socket_msg.push_back(new std::string("disconnected"));
	m_socket_msg.push_back(new std::string("error"));
}


// dTor
Server::~Server()
{
	SAFE_DEL(m_socket);			SAFE_DEL(m_packet);
	SAFE_DEL(m_server_IP);		SAFE_DEL(m_sender_IP);

	auto& itr = m_client_map.begin();
	if (itr != m_client_map.end())
	{
		SAFE_DEL(itr->second);
		m_client_map.erase(itr);
	}

	for (auto& msg : m_socket_msg)
	{
		SAFE_DEL(msg);
	}
}


// recieve packets
void Server::receive_packet()
{
	m_packet->clear();
	while (m_socket->receive(*m_packet, *m_sender_IP, m_sender_port) != sf::Socket::Done) {}

	for (auto& itr : m_client_map)
	{
		if (m_sender_IP->toString() == *itr.second->get_IP() &&
			m_sender_port == itr.second->get_port())
		{
			itr.second->set_packet(*m_packet);
		}
	}

	//packet_status('r', m_socket_status);	// 'r' is key for (r)ecieve messages
}


// update packets
void Server::update_packet()
{
	for (auto& itr : m_client_map)	// update pos, health etc etc
	{
		//if (!itr.second->get_is_connected)


		itr.second->update();
	}

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
			}
		}
	}
}


// send packets
void Server::send_packet()
{
	for (auto& itr : m_client_map)
	{
		m_socket_status = m_socket->send(*itr.second->get_packet(), 
										*itr.second->get_IP(),
										itr.second->get_port());

		//packet_status('s', m_socket_status);	// 's' is key for (s)end messages
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

			m_socket_status = m_socket->send(*m_client_map[sender_port]->get_packet(),	// send init packet
											*m_client_map[sender_port]->get_IP(),
											m_client_map[sender_port]->get_port());

			std::cout << "Client " << m_nr_clients_connected << " connected." <<
						"\nIP: " << *m_client_map.find(sender_port)->second->get_IP() <<
						"\nPort: " << m_client_map.find(sender_port)->second->get_port() <<
						'\n' << std::endl;
		}
	} while (m_client_map.size() < 2);

	std::cout << "Player connections established" << std::endl;

	// TESTING
	std::string test = "test";

	for (auto& itr : m_client_map)
		while (m_socket->send(test.c_str(), test.size() + 1, *itr.second->get_IP(), itr.second->get_port()) != sf::Socket::Done);
}


// run connect
void Server::run_connect()
{
	std::cout << "Starting game in..\n" << std::endl;

	while (true)	// fix this
	{
		receive_packet();
		update_packet();
		send_packet();
	}
}


// run
void Server::run()
{
	init_connect();
	run_connect();
}
