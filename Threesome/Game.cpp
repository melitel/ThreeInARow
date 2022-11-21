#include "Game.h"
#include<iostream>

sf::Color g_colors[] = {
	sf::Color::Yellow, sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Magenta
};

const uint32_t INVALID_INDEX = uint32_t(-1);

void Game::run()
{

	initialize();

	m_selected_index_1 = INVALID_INDEX;
	m_selected_index_2 = INVALID_INDEX;

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

void Game::initialize()
{
	uint32_t n = 8;
	float board_size = (n * (m_gem_size * 2)) + ((n + 1) * gem_offset);
	m_board.setSize(sf::Vector2f(board_size, board_size));
	m_board.setFillColor(sf::Color::Black);
	m_board.setPosition(sf::Vector2f(0.f, 0.f));
	m_window.draw(m_board);

	for (uint32_t i = 0; i < n; i++) {

		const float offset_gem_i = gem_offset + (m_gem_size*2 + gem_offset) * i;
		
		for (uint32_t j = 0; j < n; j++)
		{
			const int gem_index = i + j * n;
			const float offset_gem_j = gem_offset + (m_gem_size*2 + gem_offset) * j;			

			Gem& gem = m_gems_array[gem_index];
			gem.color = (gem_color)distr(gen);
			gem.circle.setRadius(m_gem_size);
			gem.circle.setFillColor(g_colors[gem.color]);
			gem.circle.setPosition(m_board.getPosition()+sf::Vector2f(offset_gem_i, offset_gem_j));
			
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
		m_window.draw(m_gems_array[i].circle);
	}

	m_window.display();
}

void Game::update()
{
	if (m_selected_index_1 != INVALID_INDEX && m_selected_index_2 != INVALID_INDEX) {

		gem_color color = m_gems_array[m_selected_index_1].color;
		m_gems_array[m_selected_index_1].color = m_gems_array[m_selected_index_2].color;
		m_gems_array[m_selected_index_2].color = color;

		m_selected_index_1 = INVALID_INDEX;
		m_selected_index_2 = INVALID_INDEX;
	}

	for (int i = 0; i < m_gems_array.size(); i++)
	{
		Gem& gem = m_gems_array[i];
		gem.circle.setFillColor(g_colors[gem.color]);
	}
	
}

void Game::gather_input(const sf::Vector2i &mouse_position) {

	uint32_t index = get_index_by_mouse_position(mouse_position);


	if (index != INVALID_INDEX && m_selected_index_1 == INVALID_INDEX)
	{
		m_selected_index_1 = index;

	}
	else if (index != INVALID_INDEX && m_selected_index_2 == INVALID_INDEX && m_selected_index_1 != INVALID_INDEX) {

		m_selected_index_2 = index;
	}
}

uint32_t Game::get_index_by_mouse_position(const sf::Vector2i& mouse_position)
{
	for (int i = 0; i < m_gems_array.size(); i++)
	{
		const Gem& gem = m_gems_array[i];
		sf::FloatRect gem_bounds = gem.circle.getGlobalBounds();

		if (gem_bounds.contains(sf::Vector2f((float)mouse_position.x, (float)mouse_position.y))) {
			return i;
		}

	}
	return INVALID_INDEX;
}
