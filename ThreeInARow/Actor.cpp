#include "Actor.h"
#include<iostream>
#include <cmath>

void Actor::initialize_actor(const sf::Vector2f & pos, const std::string & name)
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


void Actor::initialize_actor_hp(const sf::Vector2f& pos)
{
	m_hp_bar.setPosition(pos);
	m_hp_bar.setSize(sf::Vector2f(m_hp_bar_size, 15.f));
	m_hp_bar.setFillColor(sf::Color::Green);
}

void Actor::initialize_actor_name(const sf::Vector2f& pos, const std::string& name, const std::string& font_name)
{
	if (!m_font.loadFromFile(font_name))
	{
		//error::
	}
	m_actor_name.setCharacterSize(25);
	m_actor_name.setFillColor(sf::Color::Black);
	m_actor_name.setPosition(pos);
	m_actor_name.setFont(m_font);
	m_actor_name.setString(name);

}

void Actor::initialize_actor_color(float offset_player_gem_x, float offset_player_gem_y, uint32_t gem_color_id, sf::Texture &gem_texture)
{
	m_actor_gem_color_id = gem_color_id;
	m_actor_gem.setPosition(sf::Vector2f(offset_player_gem_x, offset_player_gem_y));
	m_actor_gem.setTexture(&gem_texture);
	m_actor_gem.setSize(sf::Vector2f(m_gem_size, m_gem_size));

}

void Actor::draw(std::unique_ptr<sf::RenderWindow>& window)
{
	window->draw(m_actor);
	window->draw(m_hp_bar);
	window->draw(m_actor_name);
	window->draw(m_actor_gem);

}

uint32_t Actor::return_actor_gem_color() {

	return m_actor_gem_color_id;
}


void Actor::decrease_hp(uint32_t decrease_coefficient, bool update_pos) {

	//m_hp_bar_size = 380;
	int32_t hp_left = m_hp_bar_size - (100 * decrease_coefficient);

	if (hp_left < 0)
	{
		hp_left = 0;
	}

	m_hp_bar.setSize(sf::Vector2f(hp_left, 15.f));
	if (update_pos) {
		auto pos = m_hp_bar.getPosition();
		pos.x += 10 * decrease_coefficient;
		m_hp_bar.setPosition(pos);
	}
	
	m_hp_bar_size = hp_left;
}

uint32_t Actor::return_actor_hp_left() {

	return m_hp_bar_size;
}

