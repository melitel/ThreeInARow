#pragma once
#include "Actor.h"
#include"Animation.h"
#include "Board.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>

class Game
{
public:
	void run();

private:

	Actor m_player;
	Actor m_monster;
	Board m_play_board;

	void initialize();
	void draw();
	void update();	

	void gather_input(const sf::Vector2i& mouse_position);	

	enum class player { hero, monster };
	player m_player_turn{ player::hero };

	void damage_dealing();

	enum game_state {
		gs_waiting_for_move,
		gs_processing_move,
		gs_destroying,
		gs_swapping,
		gs_round_finished
	};
	
	game_state m_game_state;	

	//SFML graphics objects
	uint32_t m_window_width = 850;
	uint32_t m_window_height = 600;
	const uint32_t m_finish_board_width = 300;
	const uint32_t m_finish_board_height = 150;
	const float m_vs_side = 50.f;
	const float m_monster_hp_x = 460.f;
	const float m_monster_hp_y = 50.f;
	const float m_player_hp_x = 10.f;
	const float m_player_hp_y = 50.f;
	const float actor_offset_y{ 100 };
	bool m_success_move{ false };
	uint32_t m_selected_index_1;
	uint32_t m_selected_index_2;
	uint32_t m_valid_move;

	static std::unique_ptr<sf::RenderWindow> m_window;
	sf::RectangleShape m_upper_border;
	sf::RectangleShape m_lower_border;
	//sf::RectangleShape m_board;
	sf::RectangleShape m_finish_board;
	sf::RectangleShape m_background;
	sf::RectangleShape m_vs;
	sf::RectangleShape m_gem;	
	sf::CircleShape m_hero_turn;
	sf::CircleShape m_monster_turn;	
	sf::Texture m_background_tex;
	sf::Texture m_vs_tex;
	sf::Text m_who_wins;
	sf::Font m_game_font;
	
	//time logic
	std::chrono::time_point<std::chrono::system_clock> m_time{ std::chrono::system_clock::now() };
	std::chrono::time_point<std::chrono::system_clock> m_start_time{ std::chrono::system_clock::now() };
	std::chrono::time_point<std::chrono::system_clock> start_animation_time = std::chrono::system_clock::now();
	std::chrono::duration<float> m_total_time{ 0 };
	std::chrono::duration<float> m_dt{ 0 };
	uint64_t m_frame_id{ 0 };
};
