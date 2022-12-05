#include "Animation.h"

void Animation::update(float dt)
{
	if (m_state == animation_state::as_active)
	{		
		m_animation_time += dt;
		float change_frame = m_animation_total_time / m_frames_count;

		if (m_animation_time >= m_animation_total_time) {
			m_state = as_nonactive;
			m_frame_id = m_frames_count;
		}

		if (m_animation_time >= change_frame*m_frame_id)
		{			
			++m_frame_id;
		}
	}	
}

void Animation::play()
{
	m_state = animation_state::as_active;	
	m_start_time = std::chrono::system_clock::now();
	
}

uint32_t Animation::get_frame_id() {

	return m_frame_id;

}
