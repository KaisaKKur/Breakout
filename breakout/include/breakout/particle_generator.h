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

/* 表示单个粒子及其状态 */
struct Particle {
	glm::vec2	_position;
	glm::vec2	_velocity;
	glm::vec4	_color;
	float		_life;

	Particle();
};

/* ParticleGenerator 用于渲染大量粒子
 * 在给定时间重复生成、杀死粒子
 */
class ParticleGenerator {
public:
	ParticleGenerator(sprite::Shader& shader, const sprite::Texture2D& texture, unsigned int amount);
	virtual ~ParticleGenerator();

public:
	/* 更新所有粒子 */
	inline virtual void			update(const float delta_time, GameObject& object, unsigned int new_particles, const glm::vec2& offset = glm::vec2(0.0f));
	/* 渲染所有粒子 */
	inline virtual void			draw();

private:
	/* 初始化缓冲区和顶点属性 */
	inline virtual void			init();
	/* 返回当前未使用的第一个粒子索引，例如寿命 <= 0.0f，如果当前没有任何粒子处于非活动状态 */
	inline virtual unsigned int	firstUnusedParticle();
	/* 重生粒子 */
	inline virtual void			respawnParticle(Particle& particle, GameObject& object, const glm::vec2 offset = glm::vec2(0.0f));

private:
	std::vector<Particle>	_particles;
	unsigned int			_last_use_particle; /* 存储最后使用的粒子的索引（用于快速访问下一个死粒子） */
	unsigned int			_amount;
	sprite::Shader			_shader;
	sprite::Texture2D		_texture;
	unsigned int			_vao;
};

}

#endif // !_PARTICLE_GENERATOR_H_
