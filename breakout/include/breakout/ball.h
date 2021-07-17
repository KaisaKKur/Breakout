/*!
 * \file ball.h
 * \date 2020/11/07 14:34
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
 
#ifndef _BALL_H_
#define _BALL_H_

#include <glm/glm.hpp>

#include "game_object.h"

namespace breakout {

class Ball : public GameObject {
public:
	Ball();
	Ball(const glm::vec2& position, float radius, const glm::vec2& velocity, const sprite::Texture2D& sprite, const glm::vec3& color = glm::vec3(1.0f));
	virtual ~Ball();

public:
	// 移动球，使其保持在窗口范围内（底部边缘除外）；返回新职位
	inline virtual glm::vec2	move(const float delta_time, const unsigned int window);
	//
	// 以给定的位置和速度将球重置为原始状态
	inline virtual void			reset(const glm::vec2& position, const glm::vec2& velocity);

public:
	inline virtual float		getRadius() const;
	//
	inline virtual bool			getStuck() const;
	inline virtual void			setStuck(const bool stuck);
	//
	inline virtual bool			getSticky() const;
	inline virtual void			setSticky(const bool sticky);
	//
	inline virtual bool			getPassThrough() const;
	inline virtual void			setPassThrough(const bool pass_through);

private:
	float	_radius;
	bool	_stuck;
	bool    _sticky;
	bool	_pass_through;
};

}

#endif // !_BALL_H_
