#include "Game.h"
#include<iostream>
#include <cmath>
#include <cassert>
#include "Random.h"

#define DEBUG_COLORS 0
std::unique_ptr<sf::RenderWindow> Game::m_window = nullptr;

const uint32_t INVALID_INDEX = uint32_t(-1);
std::array<sf::Color, 6> outline_color = {
	sf::Color::Yellow,
	sf::Color::Green,
	sf::Color::Red,
	sf::Color::Blue,
	sf::Color::Magenta,
	sf::Color::White
};

void Game::run()
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_window_width, m_window_height), "Three in a Row");

	initialize();

	m_selected_index_1 = INVALID_INDEX;
	m_selected_index_2 = INVALID_INDEX;
	m_valid_move = INVALID_INDEX;
	m_window->setKeyRepeatEnabled(false);
	
		while (m_window->isOpen())
		{			
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (m_window->pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					m_window->close();

				if (event.type == sf::Event::MouseButtonReleased)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i mouse_position = sf::Mouse::getPosition(*m_window);
						gather_input(mouse_position);
					}
				}
			}

			update();
			draw();
		}
	}

void Game::initialize()
{
	if (!m_game_font.loadFromFile("arial.ttf"))
	{
		//error::
	}

	m_game_state = game_state::gs_waiting_for_move;
	m_player.initialize_actor(sf::Vector2f(0.f, actor_offset_y), "hero.png");
	m_monster.initialize_actor(sf::Vector2f((float)(m_window_width - m_monster.get_width()), actor_offset_y), "monster.png");
	m_background.setSize(sf::Vector2f((float)m_window_width, (float)m_window_height));
	m_background.setPosition(sf::Vector2f(0.f, 0.f));
	m_background_tex.loadFromFile("background.jpg");
	m_background.setTexture(&m_background_tex);

	m_vs.setPosition(sf::Vector2f(395.f, 35.f));
	m_vs.setSize(sf::Vector2f(m_vs_side, m_vs_side));
	m_vs_tex.loadFromFile("vs.png");
	m_vs.setTexture(&m_vs_tex);

	m_hero_turn.setPosition(sf::Vector2f(10.f, 80.f));
	m_hero_turn.setFillColor(sf::Color::Green);
	m_hero_turn.setRadius(15.f);

	m_monster_turn.setPosition(sf::Vector2f(810.f, 70.f));
	m_monster_turn.setFillColor(sf::Color::Black);
	m_monster_turn.setRadius(15.f);

	m_player.initialize_actor_hp(sf::Vector2f(m_player_hp_x, m_player_hp_y));
	m_monster.initialize_actor_hp(sf::Vector2f(m_monster_hp_x, m_monster_hp_y));

	m_player.initialize_actor_name(sf::Vector2f(15.f, 10.f), "Archer", "arial.ttf");
	m_monster.initialize_actor_name(sf::Vector2f(770.f, 10.f), "Mage", "arial.ttf");

	m_player.initialize_actor_color(105.f, 10.f);
	m_monster.initialize_actor_color(720.f, 10.f);

	m_finish_board.setSize(sf::Vector2f(m_finish_board_width, m_finish_board_height));
	m_finish_board.setFillColor(sf::Color::Black);
	m_finish_board.setPosition(sf::Vector2f(275.f, 225.f));
	m_who_wins.setCharacterSize(25);
	m_who_wins.setFillColor(sf::Color::White);
	m_who_wins.setPosition(sf::Vector2f(350.f, 275.f));
	m_who_wins.setString("Who wins?");
	m_who_wins.setFont(m_game_font);
	m_play_board.initialize();

	m_window->display();
}

void Game::draw()
{
	// clear the window with black color
	m_window->clear(sf::Color::Black);	

	if (m_game_state == game_state::gs_round_finished)
	{	
		if (m_player_turn == player::hero)
		{
			m_who_wins.setString("Archer wins!");
		}
		else
		{
			m_who_wins.setString("Mage wins!");
		}
		m_window->draw(m_background);	
		m_window->draw(m_finish_board);
		m_window->draw(m_who_wins);
	}
	else {
		m_window->draw(m_background);
		m_window->draw(m_vs);
		m_window->draw(m_hero_turn);
		m_window->draw(m_monster_turn);	
		m_player.draw(m_window);
		m_monster.draw(m_window);	
		m_play_board.draw(m_window);
	}

	m_window->display();
}

void Game::update()
{	
	// Time logic 
	std::chrono::time_point t = std::chrono::system_clock::now();
	m_dt = t - m_time;
	m_time = t;
	m_total_time = t - m_start_time;
	float delta = std::min(m_dt.count(), 1.f/30.f);	

	if (m_selected_index_1 != INVALID_INDEX && m_selected_index_2 != INVALID_INDEX) {

		Gem::gem_color color = m_play_board.get_color(m_selected_index_1);

		m_play_board.gem_colors_swap(m_selected_index_1, m_selected_index_2, color, true);

		m_valid_move = m_play_board.searchCol(m_selected_index_1, m_selected_index_2);

		if (m_valid_move == 1)
		{
			m_game_state = game_state::gs_processing_move;
			m_selected_index_1 = INVALID_INDEX;
			m_selected_index_2 = INVALID_INDEX;
			m_valid_move = INVALID_INDEX;				
								
		}
		else {
			m_play_board.gem_colors_swap(m_selected_index_1, m_selected_index_2, color, false);
			m_selected_index_1 = INVALID_INDEX;
			m_selected_index_2 = INVALID_INDEX;
		}			
	}

	if (m_game_state == game_state::gs_processing_move && !m_play_board.gems_to_destroy_empty())
	{
		m_play_board.gems_count();
		m_play_board.paint_it_black();
		m_game_state = game_state::gs_destroying;				
	}
	if (m_game_state == game_state::gs_destroying) {
		
		if (!m_play_board.gemsToDestroyBegin_animation()) {
			
			m_play_board.gems_destroy();
			m_game_state = game_state::gs_swapping;
			m_play_board.gems_swap();	
		}
	}

	if (m_game_state == game_state::gs_swapping) {

		if (!m_play_board.gems_to_destroy_empty())
		{
			m_play_board.waiting_for_animation();
		}
		else {

			damage_dealing();			

			int32_t player_hp_left = m_player.return_actor_hp_left();
			int32_t monster_hp_left = m_monster.return_actor_hp_left();

			if (player_hp_left <= 0 || monster_hp_left <= 0)
			{			
				m_game_state = game_state::gs_round_finished;
			}
			else {
				if (m_player_turn == player::hero)
				{
					m_player_turn = player::monster;
					m_player.initialize_actor_color(105.f, 10.f);
					m_monster_turn.setFillColor(sf::Color::Green);
					m_hero_turn.setFillColor(sf::Color::Black);
				}
				else {
					m_player_turn = player::hero;
					m_monster.initialize_actor_color(720.f, 10.f);
					m_hero_turn.setFillColor(sf::Color::Green);
					m_monster_turn.setFillColor(sf::Color::Black);
				}
				
				m_game_state = game_state::gs_waiting_for_move;
			}
		}
	}

	m_play_board.update(delta);
}


void Game::gather_input(const sf::Vector2i& mouse_position) {
	if (m_game_state != gs_waiting_for_move) {
		return;
	}

	uint32_t index = m_play_board.get_index_by_mouse_position(mouse_position);

	if (index != INVALID_INDEX && m_selected_index_1 == INVALID_INDEX)
	{
		m_selected_index_1 = index;
		m_play_board.set_outline_thickness(m_selected_index_1, 2);
	}
	else if (index != INVALID_INDEX && m_selected_index_2 == INVALID_INDEX && m_selected_index_1 != INVALID_INDEX) {

		if (std::abs((int)m_selected_index_1 - (int)index) == 1 || std::abs((int)m_selected_index_1 - (int)index) == 8)
		{
			m_selected_index_2 = index;
		}
		else if (m_selected_index_1 == index) {
			m_play_board.set_outline_thickness(m_selected_index_1, 0);
			m_selected_index_1 = INVALID_INDEX;
		}
	}
}

void Game::damage_dealing() {

	if (m_player_turn == player::hero)
	{		
		if (m_play_board.is_doing_damage(m_player.return_actor_gem_color()))
		{
			m_monster.decrease_hp(m_play_board.decrease_coefficient(m_player.return_actor_gem_color()), true);
			m_play_board.fill_destroyed_gems(0);
		}
	}
	if (m_player_turn == player::monster)
	{
		if (m_play_board.is_doing_damage(m_monster.return_actor_gem_color()))
		{
			m_player.decrease_hp(m_play_board.decrease_coefficient(m_monster.return_actor_gem_color()), false);
			m_play_board.fill_destroyed_gems(0);
		}
	}
}
