#include "Actor.h"
#include<iostream>
#include <cmath>
#include "TextureManager.h"
#include "Random.h"

void Actor::initialize_actor(const sf::Vector2f & pos, const std::string & name)
{
	
	m_actor.setPosition(pos);
	m_actor.setSize(sf::Vector2f((float) m_hero_width, (float) m_hero_height));
	m_actor_texture = &TextureManager::GetTexture(name);
	m_actor.setTexture(m_actor_texture);
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

void Actor::initialize_actor_color(float offset_player_gem_x, float offset_player_gem_y)
{
	player_gem_color actor_gem_color = (player_gem_color)distr(gen);
	m_actor_gem.setPosition(sf::Vector2f(offset_player_gem_x, offset_player_gem_y));
	m_gems_texture = &TextureManager::GetTexture(gem_texture_filename(actor_gem_color));
	m_actor_gem.setTexture(m_gems_texture);
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
		
	int32_t hp_left = m_hp_bar_size - (10 * decrease_coefficient);

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

const std::string Actor::gem_texture_filename(player_gem_color color)
{
	std::string actorFileName;

	switch (color) {
	case player_gem_color::pgc_orange:
		actorFileName = "orange.png";
		break;
	case player_gem_color::pgc_green:
		actorFileName = "green.png";
		break;
	case player_gem_color::pgc_red:
		actorFileName = "red.png";
		break;
	case player_gem_color::pgc_blue:
		actorFileName = "blue.png";
		break;
	case player_gem_color::pgc_violet:
		actorFileName = "violet.png";
		break;	
	default:
		break;
	}

	return actorFileName;
}

