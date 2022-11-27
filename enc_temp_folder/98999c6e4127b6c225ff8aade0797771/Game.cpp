#include "Game.h"
#include<iostream>
#include <cmath>

const uint32_t INVALID_INDEX = uint32_t(-1);

void Game::run()
{

	initialize();

	m_selected_index_1 = INVALID_INDEX;
	m_selected_index_2 = INVALID_INDEX;
	m_valid_move = INVALID_INDEX;

	while (m_window.isOpen()) {


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
			}

			update();
			draw();
		}
	}

}

void Game::load_textures(std::string file_path, Game::gem_color index) {

	if (!m_colors[index].loadFromFile(file_path))
	{
		//error::
	}

}

void Game::initialize()
{
	float board_width = ((m_rows * m_gem_side) + ((m_rows + 1) * gem_offset));
	float board_heights = ((m_columns * m_gem_side) + ((m_columns + 1) * gem_offset));
	m_board.setSize(sf::Vector2f(board_width, board_heights));
	m_board.setFillColor(sf::Color::Black);
	m_board.setPosition(sf::Vector2f(0.f, 0.f));
	m_window.draw(m_board);

	load_textures("gem1.png", gc_orange);
	load_textures("gem6.png", gc_green);
	load_textures("gem3.png", gc_red);
	load_textures("gem4.png", gc_blue);
	load_textures("gem5.png", gc_violet);
	load_textures("gem2.png", gc_black);

	for (uint32_t i = 0; i < m_rows; i++) {

		const float offset_gem_i = gem_offset + (m_gem_side + gem_offset) * i;

		for (uint32_t j = 0; j < m_columns; j++)
		{
			const int gem_index = i + j * m_columns;
			const float offset_gem_j = gem_offset + (m_gem_side + gem_offset) * j;

			Gem& gem = m_gems_array[gem_index];
			gem.color = (gem_color)distr(gen);
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
	m_window.clear(sf::Color::White);
	m_window.draw(m_board);

	for (int i = 0; i < m_gems_array.size(); i++)
	{
		m_window.draw(m_gems_array[i].rect);
	}

	m_window.display();
}

void Game::update()
{
	if (m_selected_index_1 != INVALID_INDEX && m_selected_index_2 != INVALID_INDEX) {

		gem_color color = m_gems_array[m_selected_index_1].color;
		m_gems_array[m_selected_index_1].color = m_gems_array[m_selected_index_2].color;
		m_gems_array[m_selected_index_2].color = color;
		m_gems_array[m_selected_index_1].rect.setOutlineThickness(0);

		/*uint32_t move_valid = 0;*/

		m_valid_move = searchCol(m_selected_index_1, m_selected_index_2);

		if (m_valid_move == 1)
		{
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

// 		for (int i = 0; i < m_gems_to_destroy.size(); ++i)
// 		{
// 			if (m_gems_to_destroy[i] == m_selected_index_1 || m_gems_to_destroy[i] == m_selected_index_2)
// 			{
// 				move_valid = 1;
// 			}			
// 		}
// 
// 		if (move_valid == 1)
// 		{
// 			m_selected_index_1 = INVALID_INDEX;
// 			m_selected_index_2 = INVALID_INDEX;
// 			move_valid = 0;
// 		}
// 		else {
// 			m_gems_to_destroy.clear();
// 			m_gems_array[m_selected_index_2].color = m_gems_array[m_selected_index_1].color;
// 			m_gems_array[m_selected_index_1].color = color;
// 			m_selected_index_1 = INVALID_INDEX;
// 			m_selected_index_2 = INVALID_INDEX;
// 		}



// 		m_gems_array[m_selected_index_1].rect.setOutlineThickness(0);
// 		m_selected_index_1 = INVALID_INDEX;
// 		m_selected_index_2 = INVALID_INDEX;

	}

	if (!m_gems_to_destroy.empty()) {
		for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
		{
			uint32_t index = *it;

			m_gems_array[index].color = gem_color::gc_black;

		}
	}

	if (!m_gems_to_destroy.empty()) {

		for (uint32_t i = 0; i < m_rows; ++i)
		{
			int index = m_columns * (m_rows - 1) + i;

			for (int j = index; j >= 0; j -= 8)
			{
				if (m_gems_array[j].color == gem_color::gc_black)
				{					
					int element_to_swap_with = j;

					while (element_to_swap_with > 0 && m_gems_array[element_to_swap_with].color == gem_color::gc_black)
					{
						element_to_swap_with -= 8;
					}

					if (element_to_swap_with < 0 || element_to_swap_with == 0)
					{
						m_gems_array[j].color = (gem_color)distr(gen);
					}
					else {
						m_gems_array[j].color = m_gems_array[element_to_swap_with].color;
						m_gems_array[element_to_swap_with].color = gem_color::gc_black;
					}

				}
			}
		}
	}

	m_gems_to_destroy.clear();

	for (int i = 0; i < m_gems_array.size(); i++)
	{
		Gem& gem = m_gems_array[i];
		gem.rect.setTexture(&m_colors[gem.color]);
	}

}

void Game::gather_input(const sf::Vector2i& mouse_position) {

	uint32_t index = get_index_by_mouse_position(mouse_position);


	if (index != INVALID_INDEX && m_selected_index_1 == INVALID_INDEX)
	{
		m_selected_index_1 = index;
		m_gems_array[m_selected_index_1].rect.setOutlineThickness(2);
		//m_gems_array[m_selected_index_1].circle.setOutlineColor(sf::Color(255, 255, 255, 255));

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
	uint32_t consegativeRepeat = 0;	
	m_gems_to_destroy.clear();
	gem_color color_1 = m_gems_array[index_1].color;
	gem_color color_2 = m_gems_array[index_2].color;
	uint32_t i = index_1;
	uint32_t j = index_2;

	while (m_gems_array[i].color == color_1 && (i + 1) % 8 == 0)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(i);
		++i;
	}

	i = index_1;

	while (m_gems_array[i].color == color_1 && i%8 == 0)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(i);
		--i;
	}

	if (consegativeRepeat < 2)
	{
		if (!m_gems_to_destroy.empty())
		{
			//delete the last added elements. Number of elements to erase should equal consegativeRepeat value
			m_gems_to_destroy.resize(m_gems_to_destroy.size() - consegativeRepeat);
		}

		consegativeRepeat = 0;
	}
	else {
		consegativeRepeat = 0;
	}

	i = index_1;

	while (m_gems_array[i].color == color_1 && i > 7)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(i);
		i -=8;
	}

	i = index_1;

	while (m_gems_array[i].color == color_1 && i < 56)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(i);
		i += 8;
	}

	if (consegativeRepeat < 2)
	{
		if (!m_gems_to_destroy.empty())
		{
			//delete the last added elements. Number of elements to erase should equal consegativeRepeat value
			m_gems_to_destroy.resize(m_gems_to_destroy.size() - consegativeRepeat);
		}
		
		consegativeRepeat = 0;
	}
	else {
		consegativeRepeat = 0;
	}

	//index_2 check

	while (m_gems_array[j].color == color_2 && (j + 1) % 8 == 0)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(j);
		++j;
	}

	j = index_2;

	while (m_gems_array[j].color == color_2 && j% 8 == 0)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(j);
		--j;
	}

	if (consegativeRepeat < 2)
	{
		if (!m_gems_to_destroy.empty())
		{
			//delete the last added elements. Number of elements to erase should equal consegativeRepeat value
			m_gems_to_destroy.resize(m_gems_to_destroy.size() - consegativeRepeat);
		}

		consegativeRepeat = 0;
	}
	else {
		consegativeRepeat = 0;
	}

	j = index_2;

	while (m_gems_array[j].color == color_2 && j > 7)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(j);
		j -=8;
	}

	j = index_2;

	while (m_gems_array[j].color == color_2 && j < 56)
	{
		++consegativeRepeat;
		m_gems_to_destroy.push_back(j);
		j += 8;
	}

	if (consegativeRepeat < 2)
	{
		if (!m_gems_to_destroy.empty())
		{
			//delete the last added elements. Number of elements to erase should equal consegativeRepeat value
			m_gems_to_destroy.resize(m_gems_to_destroy.size() - consegativeRepeat);
		}

		consegativeRepeat = 0;
	}
	else {
		consegativeRepeat = 0;
	}

	if (!m_gems_to_destroy.empty())
	{
		return 1;
	}
	else {
		INVALID_INDEX;
	}

// 	for (uint32_t i = 0; i < m_rows * m_columns; i += 8)
// 	{
// 		uint32_t consegativeRepeat = 0;
// 		gem_color color = m_gems_array[i].color;
// 
// 		for (uint32_t j = 0; j < m_rows; ++j)
// 		{
// 			uint32_t index = i + j;
// 
// 			if (m_gems_array[index].color == color) {
// 				++consegativeRepeat;
// 
// 				if ((j+1)%8 == 0 && consegativeRepeat >= 3)
// 				{
// 					for (uint32_t k = 0; k < consegativeRepeat; ++k)
// 					{
// 						m_gems_to_destroy.push_back(index - k);
// 					}
// 				}
// 			}
// 			else {
// 
// 				if (consegativeRepeat >= 3)
// 				{
// 					for (uint32_t k = 0; k < consegativeRepeat; ++k)
// 					{						
// 						m_gems_to_destroy.push_back(index - (1 + k));
// 					}
// 				}
// 
// 				consegativeRepeat = 1; //1 BECAUSE WE FOUND ONE COLOR
// 				color = m_gems_array[index].color;
// 			}
// 		}
// 	}
// 
// 	for (uint32_t i = 0; i < m_columns; ++i)
// 	{
// 		uint32_t consegativeRepeat = 0;
// 		gem_color color = m_gems_array[i].color;
// 
// 		for (uint32_t j = 0; j < m_rows * m_columns; j += 8)
// 		{
// 			uint32_t index = i + j;
// 
// 			if (m_gems_array[index].color == color) {
// 				++consegativeRepeat;
// 
// 				if (j == (m_rows*(m_columns - 1)) && consegativeRepeat >= 3)
// 				{
// 					for (uint32_t k = 0; k < consegativeRepeat; ++k)
// 					{
// 						m_gems_to_destroy.push_back(index - (8 * k));
// 					}
// 				}
// 			}
// 			else {
// 
// 				if (consegativeRepeat >= 3)
// 				{
// 					for (uint32_t k = 0; k < consegativeRepeat; ++k)
// 					{
// 						m_gems_to_destroy.push_back(index - (8 * (k + 1)));
// 					}
// 				}
// 
// 				consegativeRepeat = 1; //1 BECAUSE WE FOUND ONE COLOR
// 				color = m_gems_array[index].color;
// 			}
// 		}
// 	}
}
