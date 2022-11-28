#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <set>


class Game
{
public:
	void run();

private:

	void initialize();
	void draw();
	void update();

	void gather_input(const sf::Vector2i& mouse_position);
	uint32_t get_index_by_mouse_position(const sf::Vector2i& mouse_position);
	enum gem_color { gc_orange, gc_green, gc_red, gc_blue, gc_violet, gc_black, gc_count };
	void load_textures(std::string file_path, gem_color index);
	uint32_t searchCol(uint32_t index_1, uint32_t index_2);

	void move_check(uint32_t index, gem_color color);
	struct Gem {
		sf::RectangleShape rect;
		gem_color color;
	};

	//SFML graphics objects
	const uint32_t m_window_width = 500;
	const uint32_t m_window_height = 500;
	const float m_gem_side = 48.f;
	const float gem_offset{ 5 };
	bool m_success_move{ false };
	uint32_t m_selected_index_1;
	uint32_t m_selected_index_2;
	uint32_t m_valid_move;
	uint32_t m_rows{ 8 };
	uint32_t m_columns{ 8 };

	sf::RenderWindow m_window{ sf::VideoMode(sf::Vector2u(m_window_width, m_window_height)), "My window" };
	sf::RectangleShape m_upper_border;
	sf::RectangleShape m_lower_border;
	sf::RectangleShape m_board;
	sf::RectangleShape m_gem;
	sf::Texture m_gems_texture;	

	std::array<Game::Gem, 64> m_gems_array;
	std::array<sf::Texture, gc_count> m_colors;
	std::set<uint32_t> m_gems_to_destroy;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen{ rd() }; // seed the generator
	std::uniform_int_distribution<> distr{ 0, 4 }; // define the range
	

};

