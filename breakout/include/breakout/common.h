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
	// ��������
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
	// ��ײ�������
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
	// ��ײ���
	// --------
	// AABB-AABB ��ײ  AABB: axis-aligned bounding box
	// �����ı߽��
	bool checkCollision(GameObject& one, GameObject& two);
	//
	// AABB-Բ��ײ
	collision checkCollision(Ball& one, GameObject& two);
	// ----------------------------------------------------


	// ��ײ�ֱ�--�����Լ����¶�λ
	// -------------------------
	// ��ײ����
	Direction vectorDirection(const glm::vec2& target);
	// ------------------------------------------
	// ----------------------------------------------------------------------------
}

#endif // !_COMMON_H_
