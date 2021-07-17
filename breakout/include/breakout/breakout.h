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

/* ��Ϸ�����ʼ�� */
struct init {
	/* ���ľ��ĳ�ʼ��С */
	const glm::vec2 _PLAYER_SIZE;
	/* ���ľ��ĳ�ʼ�ٶ� */
	const float _PLAYER_VELOCITY;

	/* ��ĳ�ʼ�ٶ� */
	const glm::vec2 _INITIAL_BALL_VELOCITY;
	/* ��İ뾶 */
	const float _BALL_RADIUS;

	init() : _PLAYER_SIZE(100.0f, 20.0f), _PLAYER_VELOCITY(500.0f), _INITIAL_BALL_VELOCITY(100.0f, -350.0f), _BALL_RADIUS(12.5f) {
	}
};

/* ������Ϸ�ĵ�ǰ״̬ */ 
enum class GameState {
	ACTIVE, MENU, WIN, LOSE
};

class Breakout {
public:
	Breakout(const unsigned int , const unsigned int height);
	virtual ~Breakout();

public:	
	inline virtual void		init(); /* ��ʼ����Ϸ״̬������������ɫ��/����/���� */
	// ��Ϸѭ��
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
	inline virtual bool		isOtherPowerupActive(const std::vector<PowerUp>& powerups, PowerUpType type); /* ����Ƿ�����һ����ͬ���͵ļӵ� */
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
	GameState									_state; /* ��Ϸ״̬ */
	std::array<bool, 1024>						_keys; /* ������� */
	unsigned int								_width;
	unsigned int								_height;
	std::unique_ptr<Checkpoint>					_menu_bricks; /* �˵���Ƭ */
	std::vector<Checkpoint>						_checkpoints; /* ��Ϸ�ȼ�����ͼ�� */
	unsigned int								_checkpoint;	/* ��Ϸ�ȼ�����ͼ������ */
	std::unique_ptr<sprite::SpriteRenderer>		_renderer; /* ��Ⱦ�� */
	std::unique_ptr<GameObject>					_player; /* ��Ҷ���--ľ�� */
	std::unique_ptr<Ball>						_ball; /* ��Ϸ����--�� */
	const std::unique_ptr<const struct init>	_init; /* ��ʼ�� ľ����� */
	std::unique_ptr<ParticleGenerator>			_particles; /* ���� */
	std::unique_ptr<sprite::PostProcessor>		_effect; /* ���ڴ���Ч�� */
	std::vector<PowerUp>						_powerups; /* ��Ϸ����Ч������ */
	std::unique_ptr<audio::AudioEngine>			_audio_engine; /* ��Ƶ���� */
	bool										_audioplayer; /* ���������� */
};

}

#endif // _BREAKOUT_H_
