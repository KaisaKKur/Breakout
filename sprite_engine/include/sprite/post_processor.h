/*!
 * \file post_processor.h
 * \date 2020/11/26 16:53
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
 
#ifndef _POST_PROCESSOR_H_
#define _POST_PROCESSOR_H_

#include <vector>

#include "texture2d.h"

namespace sprite {

// 添加效果：摇动，混淆和混乱
// 抖动：稍微模糊一下场景。
// 混淆：反转场景的颜色，也反转x和y轴。
// 混沌：利用边缘检测内核创建有趣的视觉效果，并以圆形方式移动纹理图像以获得有趣的混沌
class SPRITEENGINE_API PostProcessor {
public:
	PostProcessor(sprite::Shader& shader, const unsigned int width, const unsigned int height);

public:
	// 在渲染游戏之前准备后处理器的帧缓冲区操作
	inline virtual void beginRender();
	// 在渲染游戏后调用，将所有渲染的数据存储到纹理对象中
	inline virtual void endRender();
	// 渲染PostProcessor纹理四边形（作为包含屏幕的大精灵）
	inline virtual void render(const float time);
	inline virtual void reset();

private:
	inline virtual void initRenderData();

public:
	inline virtual void		setShake(bool shake);
	//
	inline virtual bool		getConfuse() const;
	inline virtual void		setConfuse(const bool confuse);
	//
	inline virtual bool		getChaos() const;
	inline virtual void		setChaos(const bool chaos);
	//
	inline virtual float	getShakeEffectTime() const;
	inline virtual void		setShakeEffectTime(const float effect_time);

private:
	sprite::Shader		_shader;
	sprite::Texture2D	_texture;
	unsigned int		_width;
	unsigned int		_height;
	bool				_chaos;
	bool				_confuse;
	bool				_shake;
	unsigned int		_msfbo;
	unsigned int		_fbo;
	unsigned int		_rbo;
	unsigned int		_vao;
	std::vector<float>	_effect_times;
};

}

#endif // !_POST_PROCESSOR_H_
