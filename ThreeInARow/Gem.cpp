#include "Gem.h"
#include "TextureManager.h"
#include "Random.h"

void Gem::initialize(sf::Vector2f pos)
{
	m_color = (gem_color)distr(gen);
	m_current_gem_status = gem_status::no_animation;
	m_gem.setSize({ m_gem_side , m_gem_side });
	m_gem_texture = &TextureManager::GetTexture(gem_texture_filename(m_color));
	m_gem.setTexture(m_gem_texture);
	m_gem.setPosition(pos);
}

void Gem::draw(std::unique_ptr<sf::RenderWindow>& window)
{
	window->draw(m_gem);
}

void Gem::update(float dt)
{	
	m_animation.update(dt);
	uint32_t frame_id = m_animation.get_frame_id();
	uint32_t animation_id = m_animation.get_animation_id();
	if (m_current_gem_status == gem_status::no_animation)
	{
		frame_id = 0;
		animation_id = 0;
	}
	m_rectSourceSprite.left = m_sprite_width * frame_id;
	m_rectSourceSprite.top = m_sprite_width * animation_id;
	m_gem_texture = &TextureManager::GetTexture(gem_texture_filename(m_color));
	m_gem.setTexture(m_gem_texture);
	m_gem.setTextureRect(m_rectSourceSprite);
}

void Gem::change_color(gem_color color)
{
	m_color = color;
}

void Gem::change_gem_status(gem_status status)
{
	m_current_gem_status = status;
}

void Gem::play_animation(int id)
{
	m_animation.play(id);
}

bool Gem::animation_is_playing()
{
	return m_animation.is_playing();
}

void Gem::to_delete(bool toDelete)
{
	is_to_delete = toDelete;
}

Gem::gem_color Gem::get_random_color()
{
	return (gem_color)distr(gen);
}

void Gem::set_outline_thickness(int i)
{
	m_gem.setOutlineThickness(i);
}

std::string Gem::gem_texture_filename(gem_color color)
{
	std::string textureFileName;

	switch (color)
	{
	case Gem::gc_orange:
		textureFileName = "gem_sprite1.png";
		break;
	case Gem::gc_green:
		textureFileName = "gem_sprite6.png";
		break;
	case Gem::gc_red:
		textureFileName = "gem_sprite3.png";
		break;
	case Gem::gc_blue:
		textureFileName = "gem_sprite4.png";
		break;
	case Gem::gc_violet:
		textureFileName = "gem_sprite5.png";
		break;
	case Gem::gc_black:
		textureFileName = "gem_sprite2.png";
		break;
	default:
		break;
	}

	return textureFileName;
}




