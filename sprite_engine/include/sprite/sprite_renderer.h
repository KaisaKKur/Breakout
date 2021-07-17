/*!
 * \file sprite_renderer.h
 * \date 2020/10/30 22:44
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

#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include <vector>

#include "shader.h"
#include "texture2d.h"

namespace sprite {

class SPRITEENGINE_API SpriteRenderer {
public:
	SpriteRenderer(Shader& shader);
	virtual ~SpriteRenderer();

public:
	inline virtual void		drawSprite(const Texture2D& Texture2D, const glm::vec2& position, const glm::vec2& size = glm::vec2(10.0f, 10.0f), const float rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f));

private:
	inline virtual void		initRenderData();

private:
	Shader			_shader;
	unsigned int	_vao;
};

}

#endif // !_SPRITE_RENDERER_H_
