#include "PlayerServer.h"


// CTor
PlayerServer::PlayerServer()
{
	init_shape(30, 4, sf::Color::Yellow);
}


// update
void PlayerServer::update()
{
	set_shape_pos(get_cpp_server_pos_other());
	set_health(get_cpp_server_health_other());
}