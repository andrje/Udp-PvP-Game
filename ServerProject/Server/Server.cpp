#include <iostream>

#include "Server.h"

// forward
#include "../ServerClient/ServerClient.h"
#include "../ServerPlayerPacket/ServerPlayerPacket.h"


// cTor
Server::Server(const std::string & serverIP, const unsigned short serverPort)
	: m_socket(new sf::UdpSocket()),
	m_server_IP(new std::string(serverIP)),
	m_server_port(serverPort),
	m_nr_clients_connected(0),
	m_tickrate(0.0083), 
	m_current_t(0),
	m_last_t(0),
	m_clock(new sf::Clock())
{
	m_socket->bind(m_server_port);
	m_socket->setBlocking(true);

	m_socket_msg.push_back(new std::string("done"));
	m_socket_msg.push_back(new std::string("not ready"));
	m_socket_msg.push_back(new std::string("disconnected"));
	m_socket_msg.push_back(new std::string("error"));
}


// dTor
Server::~Server()
{
	SAFE_DEL(m_socket);			SAFE_DEL(m_server_IP);		SAFE_DEL(m_clock);

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


// tick rate
bool Server::update_tick(const float tickRate)
{
	m_current_t = m_clock->getElapsedTime().asSeconds();

	bool do_update = (m_current_t - m_last_t) > tickRate ? true : false;

	if (do_update)
		m_last_t = m_current_t;

	return do_update;
}


// recieve packets
void Server::recieve_packets()
{
	sf::Packet packet;
	sf::IpAddress sender_IP;
	unsigned short sender_port;

	m_socket_status = m_socket->receive(packet, sender_IP, sender_port);

	int status = socket_status('r', m_socket_status);	// 'r' is key for (r)ecieve messages

	if (status == SocketStatus::DONE)
	{
		for (auto& itr : m_client_map)
		{
			if (sender_IP.toString() == *itr.second->get_IP() &&
				sender_port ==			itr.second->get_port())
			{
				itr.second->set_packet(packet);
			}
		}
	}
}


// update packets
void Server::update_packets()
{
	sf::Vector2f pos_this;
	float health_this;

	for (auto& itr : m_client_map)	// update from player input
		itr.second->update();

	for (auto& itr_this : m_client_map)	// copy P1's P1 data to P2's P1 data, and vice versa
	{
		for (auto& itr_other : m_client_map)
		{
			if (itr_this.second->get_client_nr() !=
				itr_other.second->get_client_nr())
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
	for (auto& itr : m_client_map)
	{
		m_socket_status = m_socket->send(*itr.second->get_packet(), 
										*itr.second->get_IP(),
										itr.second->get_port());

		int status = socket_status('s', m_socket_status);	// 's' is key for (s)end messages
	}
}


// inti connect
void Server::init_connect()
{
	std::cout << "Waiting for clients...\n" << std::endl;

	sf::Packet packet;
	sf::IpAddress sender_IP;
	unsigned short sender_port;
	sf::Vector2f spawn_pos_1, spawn_pos_2;
	sf::Vector2f spawn_1(640 / 4, 360 / 2), spawn_2(640 - (640 / 4), 360 / 2);

	do
	{
		m_socket->receive(packet, sender_IP, sender_port);	// receive

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
}


// transfer packets
void Server::run_connect()
{
	std::cout << "Starting game\n" << std::endl;

	m_clock->restart();

	while (m_client_map.size() == 2)
	{
		if (update_tick(m_tickrate))
		{
			recieve_packets();
			update_packets();
			send_packets();
		}
	}
}


// socket status
int Server::socket_status(const char socketTransferType, sf::Socket::Status& status)
{
	switch (status)
	{
	case sf::Socket::Done:
		m_status_type = SocketStatus::DONE;
		break;

	case sf::Socket::NotReady:
		m_status_type = SocketStatus::NOTREADY;
		break;

	case sf::Socket::Disconnected:
		m_status_type = SocketStatus::DISCONNECTED;
		break;

	case sf::Socket::Error:
		m_status_type = SocketStatus::ERROR;
		break;

	default:
		std::cout << "Something broke in Server::socket_status()" << std::endl;
	}

	std::string type = socketTransferType == 'r' ? "recieve " : "send ";	// print socket status
	std::cout << "Socket " << type << *m_socket_msg.at(m_status_type) << std::endl;

	return m_status_type;
}


// run
void Server::run()
{
	init_connect();
	run_connect();
}
