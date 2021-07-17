/*!
 * \file particle_generator.h
 * \date 2020/11/09 22:30
 *
 * \author Kaisa.K.Kur
 * Contact: ******@***.com
 *
 */
 
/***************************************************************************
 * Copyright (C) 2020
 *
 * \brief 
 *
 * 
 *
 * \note
 ***************************************************************************/
 
#ifndef _PARTICLE_GENERATOR_H_
#define _PARTICLE_GENERATOR_H_

#include <vector>
#include <random>

#include <glm/glm.hpp>

#include <sprite/texture2d.h>

#include "game_object.h"

namespace breakout {

/* ��ʾ�������Ӽ���״̬ */
struct Particle {
	glm::vec2	_position;
	glm::vec2	_velocity;
	glm::vec4	_color;
	float		_life;

	Particle();
};

/* ParticleGenerator ������Ⱦ��������
 * �ڸ���ʱ���ظ����ɡ�ɱ������
 */
class ParticleGenerator {
public:
	ParticleGenerator(sprite::Shader& shader, const sprite::Texture2D& texture, unsigned int amount);
	virtual ~ParticleGenerator();

public:
	/* ������������ */
	inline virtual void			update(const float delta_time, GameObject& object, unsigned int new_particles, const glm::vec2& offset = glm::vec2(0.0f));
	/* ��Ⱦ�������� */
	inline virtual void			draw();

private:
	/* ��ʼ���������Ͷ������� */
	inline virtual void			init();
	/* ���ص�ǰδʹ�õĵ�һ�������������������� <= 0.0f�������ǰû���κ����Ӵ��ڷǻ״̬ */
	inline virtual unsigned int	firstUnusedParticle();
	/* �������� */
	inline virtual void			respawnParticle(Particle& particle, GameObject& object, const glm::vec2 offset = glm::vec2(0.0f));

private:
	std::vector<Particle>	_particles;
	unsigned int			_last_use_particle; /* �洢���ʹ�õ����ӵ����������ڿ��ٷ�����һ�������ӣ� */
	unsigned int			_amount;
	sprite::Shader			_shader;
	sprite::Texture2D		_texture;
	unsigned int			_vao;
};

}

#endif // !_PARTICLE_GENERATOR_H_
