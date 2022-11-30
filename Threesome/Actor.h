#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <random>
#include <set>


class Actor
{
public:
	void initialize(const sf::Vector2f& pos, const std::string& name);
	void draw(sf::RenderWindow & window);
	uint32_t get_width() const {return m_hero_width; }

private:
	const uint32_t m_hero_width = 200;
	const uint32_t m_hero_height = 420;
	sf::RectangleShape m_actor;
	sf::Texture m_actor_texture;
};

