#include "Game.h"
#include<iostream>
#include <cmath>
#include <cassert>

#define DEBUG_COLORS 0

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

	initialize();

	m_selected_index_1 = INVALID_INDEX;
	m_selected_index_2 = INVALID_INDEX;
	m_valid_move = INVALID_INDEX;
	m_window.setKeyRepeatEnabled(false);
	
		while (m_window.isOpen())
		{			
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					m_window.close();

				if (event.type == sf::Event::MouseButtonReleased)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i mouse_position = sf::Mouse::getPosition(m_window);
						gather_input(mouse_position);
					}
				}
// 				if (event.type == sf::Event::KeyPressed)
// 				{
// 					if (event.key.code == sf::Keyboard::Numpad1)
// 					{
// 						pain_it_black();
// 					}					
// 					if (event.key.code == sf::Keyboard::Numpad2)
// 					{
// 						gems_swap();
// 					}
// 					if (event.key.code == sf::Keyboard::Numpad3)
// 					{
// 						gems_fall();
// 					}					
// 				}
			}

			update();
			draw();
		}
	}


void Game::load_textures(std::string file_path, Game::gem_color index) {

	if (!m_colors[index].loadFromFile(file_path))
	{
		//error::
	}

}

void Game::load_player_gem_textures(std::string file_path, Game::player_gem_color index) {

	if (!m_player_gems_colors[index].loadFromFile(file_path))
	{
		assert(false);
	}

}

void Game::initialize()
{

	load_textures("gem_sprite1.png", gc_orange);
	load_textures("gem_sprite6.png", gc_green);
	load_textures("gem_sprite3.png", gc_red);
	load_textures("gem_sprite4.png", gc_blue);
	load_textures("gem_sprite5.png", gc_violet);
	load_textures("gem_sprite2.png", gc_black);

	load_player_gem_textures("orange.png", pgc_orange);
	load_player_gem_textures("green.png", pgc_green);
	load_player_gem_textures("red.png", pgc_red);
	load_player_gem_textures("blue.png", pgc_blue);
	load_player_gem_textures("violet.png", pgc_violet);

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

	uint32_t hero_gem_color_id = (player_gem_color)distr(gen);
	uint32_t monster_gem_color_id = (player_gem_color)distr(gen);

	m_player.initialize_actor_color(105.f, 10.f, hero_gem_color_id, m_player_gems_colors[hero_gem_color_id]);
	m_monster.initialize_actor_color(720.f, 10.f, monster_gem_color_id, m_player_gems_colors[monster_gem_color_id]);
	
	float board_width = ((m_rows * m_gem_side) + ((m_rows + 1) * gem_offset));
	float board_heights = ((m_columns * m_gem_side) + ((m_columns + 1) * gem_offset));
	m_board.setSize(sf::Vector2f(board_width, board_heights));
	m_board.setFillColor(sf::Color::Black);
	m_board.setPosition(sf::Vector2f(200.f, 100.f));		

	for (uint32_t i = 0; i < m_rows; i++) {

		const float offset_gem_i = gem_offset + (m_gem_side + gem_offset) * i;

		for (uint32_t j = 0; j < m_columns; j++)
		{
			const int gem_index = i + j * m_columns;
			const float offset_gem_j = gem_offset + (m_gem_side + gem_offset) * j;

			Gem& gem = m_gems_array[gem_index];
			gem.color = (gem_color)distr(gen);
			gem.current_gem_status = gem_status::no_animation;
			gem.rect.setSize({ m_gem_side , m_gem_side });
			gem.rect.setTexture(&m_colors[gem.color]);
			gem.rect.setPosition(m_board.getPosition() + sf::Vector2f(offset_gem_i, offset_gem_j));

		}
	}

	m_window.display();

}

void Game::draw()
{
	// clear the window with black color
	m_window.clear(sf::Color::Black);
	m_window.draw(m_board);
	m_window.draw(m_background);
	m_window.draw(m_vs);
	m_window.draw(m_hero_turn);
	m_window.draw(m_monster_turn);
	

	for (int i = 0; i < m_gems_array.size(); i++)
	{
		m_window.draw(m_gems_array[i].rect);
	}
	
	m_player.draw(m_window);
	m_monster.draw(m_window);
	m_window.display();
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

		gem_color color = m_gems_array[m_selected_index_1].color;
		m_gems_array[m_selected_index_1].color = m_gems_array[m_selected_index_2].color;
		m_gems_array[m_selected_index_2].color = color;
		m_gems_array[m_selected_index_1].rect.setOutlineThickness(0);

		m_valid_move = searchCol(m_selected_index_1, m_selected_index_2);

		if (m_valid_move == 1)
		{
			m_game_state = game_state::gs_processing_move;
			m_selected_index_1 = INVALID_INDEX;
			m_selected_index_2 = INVALID_INDEX;
			m_valid_move = INVALID_INDEX;				
								
		}
		else {
			m_gems_array[m_selected_index_2].color = m_gems_array[m_selected_index_1].color;
			m_gems_array[m_selected_index_1].color = color;
			m_selected_index_1 = INVALID_INDEX;
			m_selected_index_2 = INVALID_INDEX;
		}			
	}

	if (m_game_state == game_state::gs_processing_move && !m_gems_to_destroy.empty())
	{
		gems_count();
		pain_it_black();
		m_game_state = game_state::gs_destroying;	
			
	}
	if (m_game_state == game_state::gs_destroying) {
		
		uint32_t index = *m_gems_to_destroy.begin();			
		if (!m_gems_array[index].animation.is_playing()) {

			for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
			{
				uint32_t i = *it;
				m_gems_array[i].color = gem_color::gc_black;				
			}

			m_game_state = game_state::gs_swapping;
			gems_swap();
		}
	}

	if (m_game_state == game_state::gs_swapping) {

		if (!m_gems_to_destroy.empty())
		{

			uint32_t index = *m_gems_to_destroy.begin();

			if (!m_gems_array[index].animation.is_playing()) {

				for (int i = 0; i < m_gems_array.size(); i++)
				{
					if (m_gems_array[i].current_gem_status == gem_status::swap_animation)
					{
						uint32_t next_color = m_gems_array[i].animation.get_animation_id();
						m_gems_array[i].color = gem_color(next_color - 1);
						m_gems_array[i].current_gem_status = gem_status::no_animation;						
					}
				}

				for (int i = 0; i < m_gems_array.size(); i++) {
					auto it_set = m_gems_to_destroy.find(i);
					if (it_set != m_gems_to_destroy.end()) {
						if (m_gems_array[i].color != gem_color::gc_black) {
							m_gems_to_destroy.erase(it_set);
						}
					}
				}
				if (!m_gems_to_destroy.empty()) {
					gems_swap();					
				}				
				
			}	
		}
		else {

			damage_dealing();

			if (m_player_turn == player::hero)
			{
				m_player_turn = player::monster;
				m_monster_turn.setFillColor(sf::Color::Green);
				m_hero_turn.setFillColor(sf::Color::Black);
			}
			else {
				m_player_turn = player::hero;
				m_hero_turn.setFillColor(sf::Color::Green);
				m_monster_turn.setFillColor(sf::Color::Black);
			}

			m_game_state = game_state::gs_waiting_for_move;
		}
	}

// 	if (m_game_state == game_state::gs_gems_fall) {
// 
// 		for (int32_t i = ((m_rows * m_columns) - 1); i >= 0; i -= 8)
// 		{
// 
// 			for (int32_t j = 0; j < m_rows; ++j) {
// 
// 				int32_t index = i - j;
// 				gem_color color = m_gems_array[index].color;
// 				move_check(index, color);
// 
// 				if (!m_gems_to_destroy.empty()) {
// 					for (auto& gem_id : m_gems_to_destroy)
// 					{
// 						gem_color additional_color = m_gems_array[gem_id].color;
// 						move_check(gem_id, additional_color);
// 					}
// 
// 					pain_it_black();
// 					m_game_state = game_state::gs_destroying;
// 					break;
// 				}
// 			}
// 		}
// 		if (m_gems_to_destroy.empty()) {
// 			m_game_state = game_state::gs_waiting_for_move;
// 		}
// 	}

			
	for (int i = 0; i < m_gems_array.size(); i++)
	{
		Gem& gem = m_gems_array[i];
		gem.animation.update(delta);
		uint32_t frame_id = gem.animation.get_frame_id();
		uint32_t animation_id = gem.animation.get_animation_id();
		if (gem.current_gem_status == gem_status::no_animation)
		{
			frame_id = 0;
			animation_id = 0;
		}
		gem.rectSourceSprite.left = m_sprite_width * frame_id;
		gem.rectSourceSprite.top = m_sprite_width * animation_id;
		gem.rect.setTexture(&m_colors[gem.color]);
		gem.rect.setTextureRect(gem.rectSourceSprite);

#if DEBUG_COLORS

		gem.rect.setOutlineColor(outline_color[gem.color]);
		gem.rect.setOutlineThickness(1);

		if (0 && gem.animation.is_playing())
		{
			gem.rect.setOutlineColor(sf::Color::Red);
			gem.rect.setOutlineThickness(1);
		}
		else {
			//gem.rect.setOutlineThickness(0);
		}
#endif
	}

}


void Game::gather_input(const sf::Vector2i& mouse_position) {
	if (m_game_state != gs_waiting_for_move) {
		return;
	}

	uint32_t index = get_index_by_mouse_position(mouse_position);


	if (index != INVALID_INDEX && m_selected_index_1 == INVALID_INDEX)
	{
		m_selected_index_1 = index;
		m_gems_array[m_selected_index_1].rect.setOutlineThickness(2);

	}
	else if (index != INVALID_INDEX && m_selected_index_2 == INVALID_INDEX && m_selected_index_1 != INVALID_INDEX) {

		if (std::abs((int)m_selected_index_1 - (int)index) == 1 || std::abs((int)m_selected_index_1 - (int)index) == 8)
		{
			m_selected_index_2 = index;
		}
		else if (m_selected_index_1 == index) {
			m_gems_array[m_selected_index_1].rect.setOutlineThickness(0);
			m_selected_index_1 = INVALID_INDEX;
		}
	}
}

uint32_t Game::get_index_by_mouse_position(const sf::Vector2i& mouse_position)
{
	for (int i = 0; i < m_gems_array.size(); i++)
	{
		const Gem& gem = m_gems_array[i];
		sf::FloatRect gem_bounds = gem.rect.getGlobalBounds();

		if (gem_bounds.contains(sf::Vector2f((float)mouse_position.x, (float)mouse_position.y))) {
			return i;
		}

	}
	return INVALID_INDEX;
}

uint32_t Game::searchCol(uint32_t index_1, uint32_t index_2)
{	
	m_gems_to_destroy.clear();
	gem_color color_1 = m_gems_array[index_1].color;
	gem_color color_2 = m_gems_array[index_2].color;

	move_check(index_1, color_1);

	move_check(index_2, color_2);

	for (auto& gem_id : m_gems_to_destroy)
	{
		gem_color additional_color = m_gems_array[gem_id].color;
		move_check(gem_id, additional_color);
	}

	if (!m_gems_to_destroy.empty())
	{
		return 1;
	}
	else {
		return INVALID_INDEX;
	}
}

void Game::move_check(uint32_t index, gem_color color) {
	
	int32_t i = index;
	int32_t consegativeRepeat = 1;
	std::set<uint32_t> temp_gems_to_destroy;

	while ((index / m_columns) == (i / m_columns) && m_gems_array[i].color == color)
	{
		if (i != index) {
			++consegativeRepeat;
		}
		temp_gems_to_destroy.insert(i);
		++i;
	}

	i = index;

	while ((index / m_columns) == (i / m_columns) && m_gems_array[i].color == color)
	{
		if (i != index) {
			++consegativeRepeat;
		}
		temp_gems_to_destroy.insert(i);
		--i;
	}

	if (consegativeRepeat > 2) {
		m_gems_to_destroy.insert(temp_gems_to_destroy.begin(), temp_gems_to_destroy.end());
	}
	temp_gems_to_destroy.clear();
	consegativeRepeat = 1;
	i = index;

	while ((i >= 0) && m_gems_array[i].color == color)
	{
		if (i != index) {
			++consegativeRepeat;
		}
		temp_gems_to_destroy.insert(i);
		i -= 8;
	}

	i = index;

	while ((i < m_gems_array.size()) && m_gems_array[i].color == color)
	{
		if (i != index) {
			++consegativeRepeat;
		}
		temp_gems_to_destroy.insert(i);
		i += 8;
	}

	if (consegativeRepeat > 2) {
		m_gems_to_destroy.insert(temp_gems_to_destroy.begin(), temp_gems_to_destroy.end());
	}
	temp_gems_to_destroy.clear();
	consegativeRepeat = 1;

}

void Game::gems_count() {
	for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
	{
		uint32_t index = *it;
		if (m_gems_array[index].color == gem_color::gc_orange)
		{
			++m_destroyed_gems[0];
		}
		if (m_gems_array[index].color == gem_color::gc_green)
		{
			++m_destroyed_gems[1];
		}
		if (m_gems_array[index].color == gem_color::gc_red)
		{
			++m_destroyed_gems[2];
		}
		if (m_gems_array[index].color == gem_color::gc_blue)
		{
			++m_destroyed_gems[3];
		}
		if (m_gems_array[index].color == gem_color::gc_violet)
		{
			++m_destroyed_gems[4];
		}
	}
}

void Game:: pain_it_black() {	
	for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
	{
		uint32_t index = *it;		
		m_gems_array[index].current_gem_status = gem_status::fade_animation;
		m_gems_array[index].animation.play(0);
		m_gems_array[index].is_to_delete = true;		
	}	
}

void Game::gems_swap() {

	if (!m_gems_to_destroy.empty()) {

		for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
		{
			int32_t index = *it;

			while (index >=0)
			{
				int32_t above_id = index - 8;
				int32_t animation_id = 0;

				if (above_id < 0)
				{
					animation_id = (gem_color)distr(gen);
				}
				else {
					animation_id = m_gems_array[above_id].color;
				}

				m_gems_array[index].animation.play(animation_id + 1);	
				m_gems_array[index].current_gem_status = gem_status::swap_animation;
				index -= 8;
			}
		}
	}
}

void Game::damage_dealing() {

	if (m_player_turn == player::hero)
	{
		if (m_destroyed_gems[m_player.return_actor_gem_color()] > 0)
		{
			m_monster.decrease_hp(m_destroyed_gems[m_player.return_actor_gem_color()], true);
			m_destroyed_gems.fill(0);
		}
	}
	if (m_player_turn == player::monster)
	{
		if (m_destroyed_gems[m_monster.return_actor_gem_color()] > 0)
		{
			m_player.decrease_hp(m_destroyed_gems[m_monster.return_actor_gem_color()], false);
			m_destroyed_gems.fill(0);
		}
	}
}
