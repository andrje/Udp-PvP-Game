#include "PlayerServer.h"

#include "../../Projectile/Projectile.h"


// CTor
PlayerServer::PlayerServer()
{
	init_shape(30, 4, sf::Color::Yellow);
}


// update projectiles
void PlayerServer::update_projectiles(sf::RenderWindow & rWin, const float deltaT)
{
	//if (m_new_bullet_other)	// spawn new bullet
	//{
	//	Projectile* projectile = new Projectile(get_cpp_pos_other(), get_cpp_bullet_dir_other(), sf::Vector2f(10, 10));
	//	m_bullet_vec.push_back(projectile);

	//	m_new_bullet_other = false;
	//}


	//for (size_t i = 0; i < m_bullet_vec.size();)
	//{
	//	m_bullet_vec.at(i)->update(deltaT, rWin);	// update 

	//	if (m_bullet_vec.at(i)->destroy_self())	// destroy
	//	{
	//		SAFE_DEL(m_bullet_vec.at(i));
	//		m_bullet_vec.erase(m_bullet_vec.begin());
	//		continue;
	//	}
	//	i++;
	//}
}


// update
void PlayerServer::update(sf::RenderWindow& rWin, const float deltaT)
{
	set_shape_pos(get_cpp_pos_other());
	set_health(get_cpp_health_other());

	update_projectiles(rWin, deltaT);
}