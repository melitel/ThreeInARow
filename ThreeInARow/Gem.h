#pragma once
#include "Animation.h"
#include <random>
#include <array>

class Gem
{
public:

	enum gem_color { gc_orange, gc_green, gc_red, gc_blue, gc_violet, gc_black, gc_count };

	enum gem_status {
		no_animation,
		fade_animation,
		swap_animation
	};
	
	void initialize(sf::Vector2f pos);
	void draw(std::unique_ptr<sf::RenderWindow>& window);
	void update(float dt);
	gem_color get_color()	{ return m_color; }
	void change_color(gem_color color);
	gem_status get_status() { return m_current_gem_status; }
	void change_gem_status(gem_status status);
	void play_animation(int id);
	bool animation_is_playing();
	int get_animation_id() {
		return m_animation.get_animation_id();
	}
	void to_delete(bool toDelete);
	Gem::gem_color get_random_color();
	void set_outline_thickness(int i);
	sf::FloatRect get_global_bounds() { return m_gem.getGlobalBounds(); }	

private:

	sf::RectangleShape m_gem;
	gem_color m_color;
	Animation m_animation;
	sf::IntRect m_rectSourceSprite{ sf::Vector2i(0, 0), sf::Vector2i(500, 500) };
	gem_status m_current_gem_status;
	sf::Texture* m_gem_texture;
	std::string gem_texture_filename(gem_color color);
	const float m_gem_side = 48.f;
	const uint32_t m_sprite_width = 500;

	bool is_to_delete{ false };

};

