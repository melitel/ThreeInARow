#include "Actor.h"
#include<iostream>
#include <cmath>

void Actor::initialize(const sf::Vector2f & pos, const std::string & name)
{
	
	m_actor.setPosition(pos);
	m_actor.setSize(sf::Vector2f((float) m_hero_width, (float) m_hero_height));
	if (!m_actor_texture.loadFromFile(name))
	{
		m_actor.setFillColor(sf::Color::Red);
	}
	else {
		m_actor.setTexture(&m_actor_texture);	
	}

	
}

void Actor::draw(sf::RenderWindow& window)
{
	window.draw(m_actor);

}

