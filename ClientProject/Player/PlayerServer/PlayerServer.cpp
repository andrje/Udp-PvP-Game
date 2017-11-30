#include "PlayerServer.h"


// dTor
PlayerServer::PlayerServer()
{
	init_shape(30, 4, sf::Color::Yellow);
}


// update
void PlayerServer::update()
{
	//print_packet_struct();

	set_shape_pos(get_cpp_server_pos_other());
	set_health(get_cpp_server_health_other());
}
