/*!
 * \file game_object.h
 * \date 2020/11/05 17:45
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

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <glm/glm.hpp>

#include <sprite/texture2d.h>
#include <sprite/sprite_renderer.h>

namespace breakout {

class GameObject {
public:
	GameObject();
	GameObject(const glm::vec2& position, const glm::vec2& size, const sprite::Texture2D& sprite, const bool blend_alpha = true, const glm::vec3& color = glm::vec3(1.0f),  const glm::vec2& velocity = glm::vec2(0.0f));
	virtual ~GameObject();

public:
	inline virtual void			draw(sprite::SpriteRenderer& renderer);

public:
	inline virtual glm::vec2	getPosition() const;
	inline virtual void			setPosition(const glm::vec2& position);
	//
	inline virtual glm::vec2	getSize() const;
	inline virtual void			setSize(const glm::vec2& size);
	//
	inline virtual glm::vec2	getVelocity() const;
	inline virtual void			setVelocity(const glm::vec2& velocity);
	//
	inline virtual glm::vec3	getColor() const;
	inline virtual void			setColor(const glm::vec3& color);
	//
	inline virtual bool			getSolid() const;
	inline virtual void			setSolid(const bool solid);
	//
	inline virtual bool			getDestroyed() const;
	inline virtual void			setDestroyed(const bool destroyed);
	//
	inline virtual bool			getBlendAlpha() const;
	inline virtual void			setBlendAlpha(const bool blend_alpha);
	//
	// &
	inline virtual glm::vec2&	_getPosition();
	inline virtual glm::vec2&	_getSize();
	inline virtual glm::vec2&	_getVelocity();

protected:
	glm::vec2	_position;
	glm::vec2	_size;
	glm::vec2	_velocity;
	glm::vec3	_color;
	float		_rotation;
	bool		_solid;
	bool		_destroyed;
	bool		_blend_alpha; /* 混合纹理和着色器 alpha */
	sprite::Texture2D _sprite;
};

}

#endif // !_GAME_OBJECT_H_
