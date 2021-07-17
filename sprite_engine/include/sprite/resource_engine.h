/*!
 * \file resource_engine.h
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

#ifndef _RESOURCE_ENGINE_H_
#define _RESOURCE_ENGINE_H_

#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include <stb/stb_image.h>

#include <Kaisa/common.h>

#include "texture2d.h"

namespace sprite {

class SPRITEENGINE_API ResourceEngine {
public:
	static sprite::Shader		loadShader(const std::string_view vertex_shader_path, const std::string_view fragment_shader_path, const std::string_view geometry_shader_path, const std::string_view name);
	static sprite::Texture2D	loadTexture2D(const std::string_view path, const bool alpha, const std::string_view name);
	static void					clear();

private:
	static sprite::Shader		loadShaderFromFile(const std::string_view vertex_shader_path, const std::string_view fragment_shader_path, const std::string_view geometry_shader_path);
	static sprite::Texture2D	loadTexture2DFromFile(const std::string_view path, const bool alpha);
	
public:
	static sprite::Shader&		_getShader(const std::string_view name);
	static sprite::Texture2D&	_getTexture2D(const std::string_view name);

private:
	static std::map<std::string, sprite::Shader>	_shaders;
	static std::map<std::string, sprite::Texture2D>	_texture2ds;
};


}

#endif // !_RESOURCE_ENGINE_H_
