#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <random>
#include <set>


class Actor
{
public:
	void initialize_actor(const sf::Vector2f& pos, const std::string& name);
	void initialize_actor_hp(const sf::Vector2f& pos);
	void initialize_actor_name(const sf::Vector2f& pos, const std::string& name, const std::string& font_name);
	void initialize_actor_color(float offset_player_gem_x, float offset_player_gem_y, const sf::Texture &gem_texture);
	void draw(sf::RenderWindow & window);
	uint32_t get_width() const {return m_hero_width; }

private:
	const uint32_t m_hero_width = 200;
	const uint32_t m_hero_height = 420;
	sf::RectangleShape m_actor;
	sf::Texture m_actor_texture;
	sf::RectangleShape m_hp_bar;
	const uint32_t m_hp_bar_size = 380;
	sf::Text m_actor_name;
	sf::Font m_font;
	sf::RectangleShape m_actor_gem;
	const float m_gem_size = 35.f;
};

