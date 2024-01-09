#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>

class Animation
{
public:
	void update(float dt);
	void play(uint32_t animation_id);
	void reset() { m_frame_id = 0; m_animation_id = 0; }
	bool is_playing() const { return m_state == as_active; }


	uint32_t get_frame_id();
	uint32_t get_animation_id();
private:
	enum animation_state {as_active, as_nonactive};
	animation_state m_state{as_nonactive};
	std::chrono::time_point<std::chrono::system_clock> m_start_time{ std::chrono::system_clock::now() };
	float m_animation_time{ 0 };
	float m_animation_total_time{ 0.5 };
	sf::IntRect m_rectSourceSprite{ sf::Vector2i(0, 0), sf::Vector2i(500, 500) };
	sf::Texture m_texture;
	const uint32_t m_texture_width = 500;
	const uint32_t m_texture_height = 500;
	uint32_t m_frame_id{ 0 };
	uint32_t m_frames_count{ 8 };
	uint32_t m_animation_id{ 0 };
};

