/*!
 * \file checkpoint.h
 * \date 2020/11/06 15:11
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

#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

#include <vector>

#include "game_object.h"
#include "resource_manager.h"

namespace breakout {

class Checkpoint {
public:
	Checkpoint();

public:
	// 从文件加载级别
	inline virtual void		loadCheckpoint(const unsigned int checkpoint, const std::vector<std::vector<int>>& tile_data, const unsigned int width, const unsigned int height, const bool blend_alpha = true);
	// 渲染级别
	inline virtual void		draw(sprite::SpriteRenderer& renderer);
	// 检查关卡是否完成（所有非实心砖都被破坏）
	inline virtual bool		isCompleted();
	inline virtual void		runTick();

private:
	// 从图块数据初始化级别
	inline virtual void		init(const unsigned int checkpoint, const bool blend_alpha, const std::vector<std::vector<int>>& tile_data, const unsigned int width, const unsigned int height);
	inline virtual void		resetTick();

public:
	inline virtual std::vector<GameObject>	getBricks() const;
	inline virtual unsigned int				getCount() const;
	inline virtual unsigned int				getTick() const;
	inline virtual void						setTick(const unsigned int tick);
	//
	// &
	inline virtual std::vector<GameObject>&	_getBricks();

private:
	std::vector<GameObject>	_bricks;
	unsigned int			_count;
	unsigned int			_tick;
};

}

#endif // !_CHECKPOINT_H_
