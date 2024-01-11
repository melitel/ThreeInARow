#pragma once
#include "Gem.h"
#include <set>

class Board
{
public: 
	
	void initialize();
	void draw(std::unique_ptr<sf::RenderWindow>& window);
	void update(float dt);

	void move_check(uint32_t index, Gem::gem_color color);
	void gems_count();
	void paint_it_black();
	void gems_swap();
	void gems_destroy();
	uint32_t searchCol(uint32_t index_1, uint32_t index_2);
	bool gems_to_destroy_empty() { return m_gems_to_destroy.empty(); }
	uint32_t get_index_by_mouse_position(const sf::Vector2i& mouse_position);

	void set_outline_thickness(uint32_t id, uint32_t outline_id);
	bool is_doing_damage(uint32_t id);
	uint32_t decrease_coefficient(uint32_t id);
	void fill_destroyed_gems(uint32_t i);
	void gem_colors_swap(uint32_t index_1, uint32_t index_2, Gem::gem_color color, bool swap);
	void waiting_for_animation();
	Gem::gem_color get_color(uint32_t id) {
		return m_gems_array[id].get_color();
	}
	bool gemsToDestroyBegin_animation();

private:

	sf::RectangleShape m_board;
	uint32_t m_rows{ 8 };
	uint32_t m_columns{ 8 };
	const float m_gem_side = 48.f;
	const float gem_offset{ 5 };
	std::array<Gem, 64> m_gems_array;	
	std::array<uint32_t, Gem::gem_color::gc_count> m_destroyed_gems;
	std::set<uint32_t> m_gems_to_destroy;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen{ rd() }; // seed the generator
	std::uniform_int_distribution<> distr{ 0, 4 }; // define the range
};

