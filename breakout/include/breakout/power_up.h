/*!
 * \file power_up.h
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

#ifndef _POWER_UP_H_
#define _POWER_UP_H_

#include "game_object.h"

namespace breakout {

enum class PowerUpType {
	SPEED, STICKY, PASS_THROUNGH, PAD_SIZE_INCREASE, CONFUSE, CHAOS
};

class PowerUp : public GameObject {
public:
	PowerUp(const PowerUpType& type, const glm::vec3& color, float duration, const glm::vec2& position, const sprite::Texture2D& texture);

public:
	inline virtual PowerUpType	getType() const;
	//
	inline virtual float		getDuration() const;
	inline virtual void			setDuration(const float duration);
	//
	inline virtual bool			getActivated() const;
	inline virtual void			setActivated(const bool activated);

private:
	PowerUpType	_type;
	float		_duration;
	bool		_activated;
};

}

#endif // !_POWER_UP_H_
