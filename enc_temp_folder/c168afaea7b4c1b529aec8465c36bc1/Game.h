#pragma once
#include "Actor.h"
#include"Animation.h"
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

	Actor m_player;
	Actor m_monster;
	Actor m_hp_bar_hero;
	Actor m_hp_bar_monster;
	Actor m_hero_name;
	Actor m_monster_name;
	Actor m_hero_gem;
	Actor m_monster_gem;

	void initialize();
	void draw();
	void update();
	
	void pain_it_black();
	void gather_input(const sf::Vector2i& mouse_position);
	uint32_t get_index_by_mouse_position(const sf::Vector2i& mouse_position);
	enum gem_color { gc_orange, gc_green, gc_red, gc_blue, gc_violet, gc_black, gc_count };
	enum player_gem_color { pgc_orange, pgc_green, pgc_red, pgc_blue, pgc_violet, pgc_count };
	enum gem_status {
		no_animation,
		fade_animation,
		swap_animation
	};
	
	void load_textures(std::string file_path, gem_color index);
	void load_player_gem_textures(std::string file_path, Game::player_gem_color index);
	uint32_t searchCol(uint32_t index_1, uint32_t index_2);

	void move_check(uint32_t index, gem_color color);
	void gems_fall();
	void gems_swap();
	struct Gem {
		sf::RectangleShape rect;
		gem_color color;
		Animation animation;
		sf::IntRect rectSourceSprite{ sf::Vector2i(0, 0), sf::Vector2i(500, 500) };
		gem_status current_gem_status;

		bool is_to_delete{ false };
	};
	enum game_state {
		gs_waiting_for_move,
		gs_processing_move,
		gs_destroying,
		gs_swapping,
		gs_gems_fall
	};
	
	game_state m_game_state;
	

	//SFML graphics objects
	const uint32_t m_window_width = 850;
	const uint32_t m_window_height = 600;
	const float m_gem_side = 48.f;
	const float m_vs_side = 50.f;
	const float gem_offset{ 5 };
	const float actor_offset_y{ 100 };
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
	sf::RectangleShape m_background;
	sf::RectangleShape m_vs;
	sf::RectangleShape m_gem;	
	sf::Texture m_gems_texture;	
	sf::Texture m_background_tex;
	sf::Texture m_vs_tex;	
	
	const uint32_t m_sprite_width = 500;
	

	std::array<Game::Gem, 64> m_gems_array;
	std::array<sf::Texture, gc_count> m_colors;
	std::array<sf::Texture, pgc_count> m_player_gems_colors;
	std::set<uint32_t> m_gems_to_destroy;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen{ rd() }; // seed the generator
	std::uniform_int_distribution<> distr{ 0, 4 }; // define the range
	
	//time logic
	std::chrono::time_point<std::chrono::system_clock> m_time{ std::chrono::system_clock::now() };
	std::chrono::time_point<std::chrono::system_clock> m_start_time{ std::chrono::system_clock::now() };
	std::chrono::time_point<std::chrono::system_clock> start_animation_time = std::chrono::system_clock::now();
	std::chrono::duration<float> m_total_time{ 0 };
	std::chrono::duration<float> m_dt{ 0 };
	uint64_t m_frame_id{ 0 };

};

