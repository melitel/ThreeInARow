#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <random>


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
	enum gem_color { gc_yellow, gc_green, gc_red, gc_blue, gc_violet };

	struct Gem {
		sf::CircleShape circle;
		gem_color color;
	};

	//SFML graphics objects
	const uint32_t m_window_width = 500;
	const uint32_t m_window_height = 500;
	const float m_gem_size = 24.f;
	const float gem_offset{ 5 };
	bool m_success_move{ false };
	uint32_t m_selected_index_1;
	uint32_t m_selected_index_2;

	sf::RenderWindow m_window{ sf::VideoMode(sf::Vector2u(m_window_width, m_window_height)), "My window" };
	sf::RectangleShape m_upper_border;
	sf::RectangleShape m_lower_border;
	sf::RectangleShape m_board;
	sf::CircleShape m_gem{ m_gem_size };

	std::array<Game::Gem, 64> m_gems_array;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen{ rd() }; // seed the generator
	std::uniform_int_distribution<> distr{ 0, 4 }; // define the range
	

};

