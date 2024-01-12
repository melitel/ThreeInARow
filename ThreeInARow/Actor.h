#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <random>
#include <set>


class Actor
{
public:
	enum player_gem_color { pgc_orange, pgc_green, pgc_red, pgc_blue, pgc_violet, pgc_count };

	void initialize_actor(const sf::Vector2f& pos, const std::string& name);
	void initialize_actor_hp(const sf::Vector2f& pos);
	void initialize_actor_name(const sf::Vector2f& pos, const std::string& name, const std::string& font_name);
	void initialize_actor_color(float offset_player_gem_x, float offset_player_gem_y);
	void draw(std::unique_ptr<sf::RenderWindow>& window);
	uint32_t return_actor_gem_color();
	void decrease_hp(uint32_t decrease_coefficient, bool update_pos);
	uint32_t return_actor_hp_left();
	uint32_t get_width() const { return m_hero_width; }

private:
	const uint32_t m_hero_width = 200;
	const uint32_t m_hero_height = 420;
	sf::RectangleShape m_actor;
	sf::Texture* m_actor_texture;
	sf::RectangleShape m_hp_bar;
	int32_t m_hp_bar_size = 380;
	sf::Text m_actor_name;
	sf::Font m_font;
	sf::RectangleShape m_actor_gem;
	sf::Texture* m_gems_texture;
	const float m_gem_size = 35.f;
	player_gem_color m_actor_gem_color_id{ pgc_orange };

	const std::string gem_texture_filename(player_gem_color color);
};

