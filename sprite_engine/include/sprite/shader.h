/*!
 * \file shader.h
 * \date 2020/11/06 15:44
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
 
#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string_view>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <Kaisa/common.h>

#include "sprtdef.h"

namespace sprite {

class SPRITEENGINE_API Shader {
public:
	Shader();

public:
	inline virtual Shader&	use();
	inline virtual void		compile(const std::string_view vertex_source, const std::string_view fragment_source, const std::string_view geometry_source = NULL_STRING);
	// Ð§ÓÃº¯Êý
	inline virtual void		setFloat		(const std::string_view name,	const float value,											const bool use_shader = false);
	inline virtual void		setInteger		(const std::string_view name,	const int value,											const bool use_shader = false);
	inline virtual void		setVector2f		(const std::string_view name,	const float x, const float y,								const bool use_shader = false);
	inline virtual void		setVector2f		(const std::string_view name,	const glm::vec2& value,										const bool use_shader = false);
	inline virtual void		setVector3f		(const std::string_view name,	const float x, const float y, const float z,				const bool use_shader = false);
	inline virtual void		setVector3f		(const std::string_view name,	const glm::vec3& value,										const bool use_shader = false);
	inline virtual void		setVector4f		(const std::string_view name,	const float x, const float y, const float z, const float w,	const bool use_shader = false);
	inline virtual void		setVector4f		(const std::string_view name,	const glm::vec4& value,										const bool use_shader = false);
	inline virtual void		setMatrix4		(const std::string_view name,	const glm::mat4& matrix,									const bool use_shader = false);

private:
	inline virtual void		checkCompileError(const unsigned int object, const std::string_view type) const;

public:
	inline virtual unsigned int getId() const;

private:
	unsigned int	_id;
};

}

#endif // _SHADER_H_
