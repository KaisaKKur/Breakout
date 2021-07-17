/*!
 * \file texture2d.h
 * \date 2020/10/30 18:14
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

#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include <GL/glew.h>

#include "shader.h"

namespace sprite {

class SPRITEENGINE_API Texture2D{
public:
	Texture2D();

public:
	inline virtual void generate(const unsigned int , const unsigned int height, const unsigned char* data);
	inline virtual void bind() const;

public:
	inline virtual unsigned int getId() const;
	inline virtual void setInternalFormat(unsigned int alpha);
	inline virtual void setImageFormat(unsigned int alpha);
	//
	// &
	inline virtual unsigned int& _getId();

private:
	unsigned int	_id;
	unsigned int	_width;
	unsigned int	_height;
	unsigned int	_internal_format;
	unsigned int	_image_format;
	unsigned int	_wrap_s;
	unsigned int	_wrap_t;
	unsigned int	_filter_min;
	unsigned int	_filter_max;
};

}

#endif // !_TEXTURE2D_H_
