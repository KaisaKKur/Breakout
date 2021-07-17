/*!
 * \file breakout.h
 * \date 2020/11/06 15:32
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

#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include <array>
#include <vector>
#include <filesystem>
#include <random>

#include <audio/audio_engine.h>

#include <sprite/post_processor.h>

#include "resource_manager.h"
#include "checkpoint.h"
#include "ball.h"
#include "particle_generator.h"
#include "power_up.h"

#include "common.h"

namespace breakout {

/* 游戏对象初始化 */
struct init {
	/* 玩家木板的初始大小 */
	const glm::vec2 _PLAYER_SIZE;
	/* 玩家木板的初始速度 */
	const float _PLAYER_VELOCITY;

	/* 球的初始速度 */
	const glm::vec2 _INITIAL_BALL_VELOCITY;
	/* 球的半径 */
	const float _BALL_RADIUS;

	init() : _PLAYER_SIZE(100.0f, 20.0f), _PLAYER_VELOCITY(500.0f), _INITIAL_BALL_VELOCITY(100.0f, -350.0f), _BALL_RADIUS(12.5f) {
	}
};

/* 代表游戏的当前状态 */ 
enum class GameState {
	ACTIVE, MENU, WIN, LOSE
};

class Breakout {
public:
	Breakout(const unsigned int , const unsigned int height);
	virtual ~Breakout();

public:	
	inline virtual void		init(); /* 初始化游戏状态（加载所有着色器/纹理/级别） */
	// 游戏循环
	inline virtual void		processInput(const float delta_time);
	inline virtual void		update(const float delta_time);
	inline virtual void		render(const float runtime);
	inline virtual void		reset();
	inline virtual void		quitCurrentCheckpoint();
	inline virtual void		loadLastQuitCheckpoint();
	inline virtual void		switchBackgroundSound();
	inline virtual unsigned int getCheckpointMaxSizelimit();

private:
	inline virtual void		collision();
	inline virtual bool		shouldSpawnPowerup(const float chance);
	inline virtual void		spawnPowerups(const GameObject& block);
	inline virtual void		activatePowerup(const PowerUp& powerup);
	inline virtual bool		isOtherPowerupActive(const std::vector<PowerUp>& powerups, PowerUpType type); /* 检查是否是另一个相同类型的加电活动 */
	inline virtual void		updatePowerup(const float delta_time);
	inline virtual void		disablePowerup();
	inline virtual void		clearPowerup();
	inline virtual void		resetCheckpoint();
	inline virtual void		resetPlayer();

public:
	inline virtual GameState	getState() const;
	inline virtual void			setState(const GameState state);
	inline virtual unsigned int getCheckpoint() const;
	inline virtual void			setCheckpoint(const unsigned int checkpoint);
	inline virtual unsigned int getCheckpointsSize();
	// &
	inline virtual std::array<bool, 1024>& _getKeys();

private:
	GameState									_state; /* 游戏状态 */
	std::array<bool, 1024>						_keys; /* 按键检测 */
	unsigned int								_width;
	unsigned int								_height;
	std::unique_ptr<Checkpoint>					_menu_bricks; /* 菜单瓦片 */
	std::vector<Checkpoint>						_checkpoints; /* 游戏等级（地图） */
	unsigned int								_checkpoint;	/* 游戏等级（地图索引） */
	std::unique_ptr<sprite::SpriteRenderer>		_renderer; /* 渲染器 */
	std::unique_ptr<GameObject>					_player; /* 玩家对象--木板 */
	std::unique_ptr<Ball>						_ball; /* 游戏对象--球 */
	const std::unique_ptr<const struct init>	_init; /* 初始化 木板和球 */
	std::unique_ptr<ParticleGenerator>			_particles; /* 粒子 */
	std::unique_ptr<sprite::PostProcessor>		_effect; /* 后期处理效果 */
	std::vector<PowerUp>						_powerups; /* 游戏运行效果道具 */
	std::unique_ptr<audio::AudioEngine>			_audio_engine; /* 音频引擎 */
	bool										_audioplayer; /* 独立播音者 */
};

}

#endif // _BREAKOUT_H_
