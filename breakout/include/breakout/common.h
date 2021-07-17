/*!
 * \file common.h
 * \date 2020/11/04 16:53
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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <array>

#include "ball.h"

namespace breakout {
	// 按键控制
	// ------------------------------------------------------------------------
	namespace key {
		enum class Key {
			BREAKOUT_KEY_SPACE	= 32,
			BREAKOUT_KEY_A		= 65,
			BREAKOUT_KEY_D		= 68,
		};
	}
	// ------------------------------------------------------------------------

	// type
	// ----------------------------------------------------------------------------
	// 碰撞方向检索
	// ------------
	enum class Direction {
		UP, 
		RIGHT,
		DOWN,
		LEFT
	};
	using collision = std::tuple<bool, Direction, glm::vec2>;
	// ------------------------------------------------------
	// ----------------------------------------------------------------------------


	// function
	// ----------------------------------------------------------------------------
	// 碰撞检测
	// --------
	// AABB-AABB 碰撞  AABB: axis-aligned bounding box
	// 轴对齐的边界框
	bool checkCollision(GameObject& one, GameObject& two);
	//
	// AABB-圆碰撞
	collision checkCollision(Ball& one, GameObject& two);
	// ----------------------------------------------------


	// 碰撞分辨--方向以及重新定位
	// -------------------------
	// 碰撞方向
	Direction vectorDirection(const glm::vec2& target);
	// ------------------------------------------
	// ----------------------------------------------------------------------------
}

#endif // !_COMMON_H_
