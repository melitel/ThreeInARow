#include "Board.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const uint32_t INVALID_INDEX = uint32_t(-1);


void Board::initialize()
{
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
						
			sf::Vector2f gem_pos = m_board.getPosition() + sf::Vector2f(offset_gem_i, offset_gem_j);
			m_gems_array[gem_index].initialize(gem_pos);
		}
	}
}

void Board::draw(std::unique_ptr<sf::RenderWindow>& window)
{
	window->draw(m_board);

	for (int i = 0; i < m_gems_array.size(); i++)
	{
		m_gems_array[i].draw(window);
	}
}

void Board::update(float dt)
{
	for (int i = 0; i < m_gems_array.size(); i++) {
		m_gems_array[i].update(dt);
	}
}

void Board::move_check(uint32_t index, Gem::gem_color color) {

	int32_t i = index;
	int32_t consegativeRepeat = 1;
	std::set<uint32_t> temp_gems_to_destroy;

	while ((index / m_columns) == (i / m_columns) && m_gems_array[i].get_color() == color)
	{
		if (i != index) {
			++consegativeRepeat;
		}
		temp_gems_to_destroy.insert(i);
		++i;
	}

	i = index;

	while ((index / m_columns) == (i / m_columns) && m_gems_array[i].get_color() == color)
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

	while ((i >= 0) && m_gems_array[i].get_color() == color)
	{
		if (i != index) {
			++consegativeRepeat;
		}
		temp_gems_to_destroy.insert(i);
		i -= 8;
	}

	i = index;

	while ((i < m_gems_array.size()) && m_gems_array[i].get_color() == color)
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

void Board::gems_count() {
	for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
	{
		uint32_t index = *it;
		if (m_gems_array[index].get_color() == Gem::gem_color::gc_orange)
		{
			++m_destroyed_gems[0];
		}
		if (m_gems_array[index].get_color() == Gem::gem_color::gc_green)
		{
			++m_destroyed_gems[1];
		}
		if (m_gems_array[index].get_color() == Gem::gem_color::gc_red)
		{
			++m_destroyed_gems[2];
		}
		if (m_gems_array[index].get_color() == Gem::gem_color::gc_blue)
		{
			++m_destroyed_gems[3];
		}
		if (m_gems_array[index].get_color() == Gem::gem_color::gc_violet)
		{
			++m_destroyed_gems[4];
		}
	}
}

void Board::paint_it_black() {
	for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
	{
		uint32_t index = *it;
		m_gems_array[index].change_gem_status(Gem::gem_status::fade_animation);
		m_gems_array[index].play_animation(0);
		m_gems_array[index].to_delete(true);
	}
}

void Board::gems_swap() {

	uint32_t index = *m_gems_to_destroy.begin();

	if (!m_gems_array[index].animation_is_playing()) {

		for (int i = 0; i < m_gems_array.size(); i++)
		{
			if (m_gems_array[i].get_status() == Gem::gem_status::swap_animation)
			{
				uint32_t next_color = m_gems_array[i].get_animation_id();
				m_gems_array[i].change_color(Gem::gem_color(next_color - 1));
				m_gems_array[i].change_gem_status(Gem::gem_status::no_animation);
			}
		}

		for (int i = 0; i < m_gems_array.size(); i++) {
			auto it_set = m_gems_to_destroy.find(i);
			if (it_set != m_gems_to_destroy.end()) {
				if (m_gems_array[i].get_color() != Gem::gem_color::gc_black) {
					m_gems_to_destroy.erase(it_set);
				}
			}
		}
		if (!m_gems_to_destroy.empty()) {

			for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
			{
				int32_t index = *it;

				while (index >= 0)
				{
					int32_t above_id = index - 8;
					int32_t animation_id = 0;

					if (above_id < 0)
					{
						animation_id = (Gem::gem_color)distr(gen);
					}
					else {
						animation_id = m_gems_array[above_id].get_color();
					}

					m_gems_array[index].play_animation(animation_id + 1);
					m_gems_array[index].change_gem_status(Gem::gem_status::swap_animation);
					index -= 8;
				}
			}
		}
	}
}

void Board::gems_destroy()
{
	for (auto it = m_gems_to_destroy.cbegin(); it != m_gems_to_destroy.cend(); ++it)
	{
		uint32_t i = *it;
		m_gems_array[i].change_color(Gem::gem_color::gc_black);
	}
}

uint32_t Board::searchCol(uint32_t index_1, uint32_t index_2)
{
	m_gems_to_destroy.clear();

	Gem::gem_color color_1 = m_gems_array[index_1].get_color();
	Gem::gem_color color_2 = m_gems_array[index_2].get_color();

	move_check(index_1, color_1);

	move_check(index_2, color_2);

	for (auto& gem_id : m_gems_to_destroy)
	{
		Gem::gem_color additional_color = m_gems_array[gem_id].get_color();
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

uint32_t Board::get_index_by_mouse_position(const sf::Vector2i& mouse_position)
{
	for (int i = 0; i < m_gems_array.size(); i++)
	{
		//const Gem& gem = m_gems_array[i];
		sf::FloatRect gem_bounds = m_gems_array[i].get_global_bounds();

		if (gem_bounds.contains(sf::Vector2f((float)mouse_position.x, (float)mouse_position.y))) {
			return i;
		}
	}
	return INVALID_INDEX;
}

void Board::set_outline_thickness(uint32_t id, uint32_t outline_id)
{
	m_gems_array[id].set_outline_thickness(outline_id);
}

bool Board::is_doing_damage(uint32_t id)
{
	if (m_destroyed_gems[id] > 0) {
		return true;
	}
	else {
		return false;
	}
}

uint32_t Board::decrease_coefficient(uint32_t id)
{
	return m_destroyed_gems[id];
}

void Board::fill_destroyed_gems(uint32_t i)
{
	m_destroyed_gems.fill(i);
}

void Board::gem_colors_swap(uint32_t index_1, uint32_t index_2, Gem::gem_color color, bool swap)
{
	if (swap) {
		m_gems_array[index_1].change_color(m_gems_array[index_2].get_color());
		m_gems_array[index_2].change_color(color);
		m_gems_array[index_1].set_outline_thickness(0);	
	}
	else if (!swap) {
		m_gems_array[index_2].change_color(m_gems_array[index_1].get_color());
		m_gems_array[index_1].change_color(color);
	}
}

void Board::waiting_for_animation()
{
	uint32_t index = *m_gems_to_destroy.begin();

	if (!m_gems_array[index].animation_is_playing()) {

		for (int i = 0; i < m_gems_array.size(); i++)
		{
			if (m_gems_array[i].get_status() == Gem::gem_status::swap_animation)
			{
				uint32_t next_color = m_gems_array[i].get_animation_id();
				m_gems_array[i].change_color(Gem::gem_color(next_color - 1));
				m_gems_array[i].change_gem_status(Gem::gem_status::no_animation);
			}
		}

		for (int i = 0; i < m_gems_array.size(); i++) {
			auto it_set = m_gems_to_destroy.find(i);
			if (it_set != m_gems_to_destroy.end()) {
				if (m_gems_array[i].get_color() != Gem::gem_color::gc_black) {
					m_gems_to_destroy.erase(it_set);
				}
			}
		}
		if (!m_gems_to_destroy.empty()) {
			gems_swap();
		}
	}
}

bool Board::gemsToDestroyBegin_animation()
{
	uint32_t index = *m_gems_to_destroy.begin();

	if (m_gems_array[index].animation_is_playing()) {
		return true;
	}
	else {
		return false;	
	}
}
