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

// ���Ч����ҡ���������ͻ���
// ��������΢ģ��һ�³�����
// ��������ת��������ɫ��Ҳ��תx��y�ᡣ
// ���磺���ñ�Ե����ں˴�����Ȥ���Ӿ�Ч��������Բ�η�ʽ�ƶ�����ͼ���Ի����Ȥ�Ļ���
class SPRITEENGINE_API PostProcessor {
public:
	PostProcessor(sprite::Shader& shader, const unsigned int width, const unsigned int height);

public:
	// ����Ⱦ��Ϸ֮ǰ׼����������֡����������
	inline virtual void beginRender();
	// ����Ⱦ��Ϸ����ã���������Ⱦ�����ݴ洢�����������
	inline virtual void endRender();
	// ��ȾPostProcessor�����ı��Σ���Ϊ������Ļ�Ĵ��飩
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
