#include "breakout/breakout.h"

namespace breakout {

Breakout::Breakout(const unsigned int width, const unsigned int height) : _state(GameState::MENU), _keys(), _width(width), _height(height), _menu_bricks(), _checkpoints(), _checkpoint(), _renderer(nullptr), _player(nullptr), _ball(nullptr), _init(new struct init), _particles(nullptr), _effect(nullptr), _powerups(), _audio_engine(new audio::AudioEngine()), _audioplayer(true) {
	loadLastQuitCheckpoint();
}

Breakout::~Breakout() {
	ResourceManager::clear();
}

inline void Breakout::processInput(const float delta_time) {
	if (_state == GameState::ACTIVE) {
		float velocity = _init->_PLAYER_VELOCITY * delta_time;
		if (_keys[static_cast<int>(key::Key::BREAKOUT_KEY_A)]) {
			if (_player->_getPosition().x >= 0.0f) {
				_player->_getPosition().x -= velocity;
				if (_ball->getStuck()) {
					_ball->_getPosition().x -= velocity;
				}
			}
		}
		if (_keys[static_cast<int>(key::Key::BREAKOUT_KEY_D)]) {
			if (_player->_getPosition().x <= _width - _player->getSize().x) {
				_player->_getPosition().x += velocity;
				if (_ball->getStuck()) {
					_ball->_getPosition().x += velocity;
				}
			}
		}
		if (_keys[static_cast<int>(key::Key::BREAKOUT_KEY_SPACE)]) {
			_ball->setStuck(false);
		}
	}
}

inline void Breakout::update(const float delta_time) {
	/* 更新球对象 */
	_ball->move(delta_time, _width);
	/* 更新粒子 */
	_particles->update(delta_time, *_ball, 10, glm::vec2(_ball->getRadius() / 2.0f));
	/* 碰撞检测 */
	collision();
	/* 通电 */
	updatePowerup(delta_time);
	/* 减少震动时间 */
	if (_effect->getShakeEffectTime() > 0.0f) {
		_effect->setShakeEffectTime(_effect->getShakeEffectTime() - delta_time);
		if (_effect->getShakeEffectTime() <= 0.0f)
			_effect->setShake(false);
	}

	// 重置关卡
	if (_ball->getPosition().y >= _height) {
		_state = GameState::LOSE;
		if (_audioplayer) {
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("lose"), false, 0.86f);
			_audioplayer = false;
		}
	}
	if (_checkpoints[_checkpoint].isCompleted()) {
		_state = GameState::WIN;
		if (_audioplayer) {
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("win"), false, 0.86f);
			_audioplayer = false;
		}
	}
	
	// Cache Checkpoint When Quit Current Checkpoint
	if (_state == GameState::MENU) {
		quitCurrentCheckpoint();
	}
	// ----------------
}

inline void Breakout::render(const float runtime) {
	if (_state == GameState::ACTIVE || _state == GameState::LOSE || _state == GameState::WIN) {
		_effect->beginRender(); {
			_renderer->drawSprite(ResourceManager::_getTexture2D(std::string(std::to_string(_checkpoint + 1)) + "bk1"), glm::vec2(0.0f, 0.0f), glm::vec2(_width, _height), 0.0f);
			_checkpoints[_checkpoint].draw(*_renderer);
			_particles->draw();
			_player->draw(*_renderer);
			_ball->draw(*_renderer);
			for (PowerUp& powerup : _powerups) {
				if (!powerup.getDestroyed()) {
					powerup.draw(*_renderer);
				}
			}
		} _effect->endRender();
		_effect->render(runtime);
	} else if (_state == GameState::MENU) {
		_renderer->drawSprite(ResourceManager::_getTexture2D("menu_bk"), glm::vec2(0.0f, 0.0f), glm::vec2(_width, _height), 0.0f);
		_menu_bricks->draw(*_renderer);
	}
}

inline void Breakout::reset() {
	clearPowerup();
	resetCheckpoint();
	resetPlayer();
	_audioplayer = true;
}

inline void Breakout::quitCurrentCheckpoint() {
	ResourceManager::cacheQuitCheckpointToFile(_checkpoint);
}

inline void Breakout::loadLastQuitCheckpoint() {
	ResourceManager::loadLastQuitCheckpointFromFile(_checkpoint);
}

inline void Breakout::switchBackgroundSound() {
	_audio_engine->stopAll();
	
	if (_state == GameState::ACTIVE) {
		switch (_checkpoint) {
		case 0:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp1"), true, 0.86f);
		break;
	
		case 1:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp2"), true, 0.86f);
		break;
	
		case 2:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp3"), true, 0.86f);
		break;
	
		case 3:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp4"), true, 0.86f);
		break;
	
		case 4:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp5"), true, 0.86f);
		break;
	
		case 5:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp6"), true, 0.86f);
		break;
	
		case 6:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp7"), true, 0.86f);
		break;
	
		case 7:
			_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("cp8"), true, 0.86f);
		break;

		default:
		break;
		}
	} else if (_state == GameState::MENU) {
		_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("main_menu"), true, 0.86f);
	}
}

inline unsigned int Breakout::getCheckpointMaxSizelimit() {
	return getCheckpointsSize();
}

inline void Breakout::collision() {
	// 球-砖
	// -----
	for (GameObject& box : _checkpoints[_checkpoint]._getBricks()) {
		if (!box.getDestroyed()) {
			breakout::collision collision = checkCollision(*_ball, box);
			if (std::get<0>(collision)) { /* 如果碰撞为真 */
				if (!box.getSolid()) { /* 如果砖块，则破坏块 */
					box.setDestroyed(true);
					_checkpoints[_checkpoint].runTick();
					spawnPowerups(box);
					_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("bleep1"));
				} else { /* 如果是固体砖 开启抖动效果 */
					_effect->setShakeEffectTime(0.05f);
					_effect->setShake(true);
					_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("solid"));
				}
				// 碰撞解决方案
				// --------
				Direction direction		= std::get<1>(collision);
				glm::vec2 difference	= std::get<2>(collision);
				// 
				// 水平碰撞
				if (!(_ball->getPassThrough() && !box.getSolid())) { // 当开启直通功能，则不做碰撞解决
					if (direction == Direction::LEFT || direction == Direction::RIGHT) {
						// 反向水平速度
						_ball->_getVelocity().x = -_ball->getVelocity().x;
						//
						/*******************************************************************************/
						// 重新定位
						float penetration = _ball->getRadius() - std::abs(difference.x);
						if (direction == Direction::LEFT) { /* 向右移动球 */
							_ball->_getPosition().x += penetration;
						} else {								/* 向左移动球 */
							_ball->_getPosition().x -= penetration;
						}
						/*******************************************************************************/
					} else { 
						// 垂直碰撞
						// 反向垂直速度
						_ball->_getVelocity().y = -_ball->_getVelocity().y;
						//
						/*******************************************************************************/
						// 重新定位
						float penetration = _ball->getRadius() - std::abs(difference.y);
						if (direction == Direction::UP) {	/* 向上移动球 */
							_ball->_getPosition().y -= penetration;
						} else {								/* 向下移动球 */
							_ball->_getPosition().y += penetration;
						}
						/*******************************************************************************/
					}
				}
			}
		}
	}
	// ----------------------------------------------------------------

	// 附件通电功能
	// ---------------------------------------------------------------------------
	for (PowerUp& powerup : _powerups) {
		if (!powerup.getDestroyed()) {
			if (powerup.getPosition().y >= _height) {
				powerup.setDestroyed(true);
			}
			if (checkCollision(*_player, powerup)) { // 与板相撞，启动电源
				activatePowerup(powerup);
				powerup.setDestroyed(true);
				powerup.setActivated(true);
				_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("powerup"));
			}
		}
	}
	// ---------------------------------------------------------------------------

	// 球-木板
	// -------
	breakout::collision result = checkCollision(*_ball, *_player); /* 始终碰撞在 UP */
	if (!_ball->getStuck() && std::get<0>(result)) { // 如果球没有被卡住
		/* 检查它撞击木板的位置，并根据撞击木板的位置改变速度 */
		float center_board	= _player->getPosition().x + _player->getSize().x / 2.0f;
		float distance		= std::abs((_ball->getPosition().x + _ball->getRadius()) - center_board);
		float percentage	= distance / (_player->getSize().x / 2.0f);
		/* 然后相应地移动 */
		float strength		= 2.0f;
		float direction		= _ball->getVelocity().x > 0 ? 1.0f : -1.0f;
		glm::vec2 old_velocity	= _ball->getVelocity();
		_ball->_getVelocity().x = direction * (_ball->getVelocity().x > _init->_INITIAL_BALL_VELOCITY.x ? _ball->getVelocity().x : _init->_INITIAL_BALL_VELOCITY.x) * percentage * strength;

		_ball->_getVelocity() = glm::normalize(_ball->getVelocity()) * glm::length(old_velocity);

		_ball->_getVelocity().y = -1.0f * std::abs(_ball->getVelocity().y);
	
		// 激活了粘效果，则每当球碰撞时，球最终都会粘在板上
		// 然后，必须再次按下空格键以释放球
		_ball->setStuck(_ball->getSticky());

		_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("bleep2"));
	}
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

inline void Breakout::resetCheckpoint() {
	bool blend_alpha = _checkpoints[_checkpoint].getBricks().at(0).getBlendAlpha();
	_checkpoints[_checkpoint].loadCheckpoint(_checkpoint, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at(std::to_string(_checkpoint + 1))), _width, _height / 2, blend_alpha);
}

inline void Breakout::resetPlayer() {
	// 重置玩家 / 球的状态
	// ------------------
	_player->setSize(_init->_PLAYER_SIZE);
	_player->setPosition(glm::vec2((_width - _init->_PLAYER_SIZE.x) / 2.0f, _height - _init->_PLAYER_SIZE.y));
	_player->setColor(glm::vec3(1.0f));
	_ball->reset(_player->getPosition() + glm::vec2(_init->_PLAYER_SIZE.x / 2.0f - _init->_BALL_RADIUS, -(_init->_BALL_RADIUS * 2.0f)), _init->_INITIAL_BALL_VELOCITY);
}

inline bool Breakout::shouldSpawnPowerup(const float chance) {
	std::random_device rd; // 将用于为随机数引擎获得种子
	std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
	std::uniform_real_distribution<> dis(1.0f, 100.0f); // 生成 [1.0, 100.0] 随机数
	
	float random = static_cast<float>(dis(gen));

	if (random <= chance) {
		return true;
	} else {
		return false;
	}
}

inline void Breakout::spawnPowerups(const GameObject& block) {
	// 正面影响
	if (shouldSpawnPowerup(9.0f)) { /* 加速 */
		_powerups.push_back(PowerUp(PowerUpType::SPEED, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.getPosition(), ResourceManager::_getTexture2D("powerup_speed")));
	}
	if (shouldSpawnPowerup(7.0f)) { /* 黏 */
		_powerups.push_back(PowerUp(PowerUpType::STICKY, glm::vec3(1.0f, 0.5f, 1.0f), 10.0f, block.getPosition(), ResourceManager::_getTexture2D("powerup_sticky")));
	}
	if (shouldSpawnPowerup(6.0f)) { /* 直通 */
		_powerups.push_back(PowerUp(PowerUpType::PASS_THROUNGH, glm::vec3(0.5f, 1.0f, 1.0f), 10.0f, block.getPosition(), ResourceManager::_getTexture2D("powerup_pass-through")));
	}
	if (shouldSpawnPowerup(7.0f)) { /* 增大板 */
		_powerups.push_back(PowerUp(PowerUpType::PAD_SIZE_INCREASE, glm::vec3(1.0f, 0.6f, 0.7f), 0.0f, block.getPosition(), ResourceManager::_getTexture2D("powerup_pad-size-increase")));
	}
	// 负面影响
	if (shouldSpawnPowerup(9.0f)) { /* 混淆 */
		_powerups.push_back(PowerUp(PowerUpType::CONFUSE, glm::vec3(0.9f, 0.5f, 0.2f), 15.0f, block.getPosition(), ResourceManager::_getTexture2D("powerup_confuse")));
	}
	if (shouldSpawnPowerup(9.0f)) { /* 混乱 */
		_powerups.push_back(PowerUp(PowerUpType::CHAOS, glm::vec3(0.8f, 0.1f, 0.1f), 15.0f, block.getPosition(), ResourceManager::_getTexture2D("powerup_chaos")));
	}
}

inline void Breakout::activatePowerup(const PowerUp& powerup) {
	if (powerup.getType() == PowerUpType::SPEED) {
		_ball->_getVelocity() *= 1.1;
	} else if (powerup.getType() == PowerUpType::STICKY) {
		_ball->setSticky(true);
		_player->setColor(glm::vec3(1.0f, 0.6f, 0.6f));
	} else if (powerup.getType() == PowerUpType::PASS_THROUNGH) {
		_ball->setPassThrough(true);
		_ball->setColor(glm::vec3(0.9f, 0.3f, 0.1f));
	} else if (powerup.getType() == PowerUpType::PAD_SIZE_INCREASE) {
		_player->_getSize().x += 10.0f;
	} else if (powerup.getType() == PowerUpType::CONFUSE) {
		if (!_effect->getConfuse()) {
			_effect->setConfuse(true);
		}
	} else if (powerup.getType() == PowerUpType::CHAOS) {
		if (!_effect->getChaos()) {
			_effect->setChaos(true);
		}
	}
}

inline bool Breakout::isOtherPowerupActive(const std::vector<PowerUp>& powerups, PowerUpType type) {
	for (const PowerUp& powerup : powerups) {
		if (powerup.getActivated()) {
			if (powerup.getType() == type) {
				return true;
			}
		}
	}
	return false;
}

inline void Breakout::updatePowerup(const float delta_time) {
	for (PowerUp& powerup : _powerups) {
		powerup._getPosition() += powerup.getVelocity() * delta_time;

		if (powerup.getActivated()) {
			powerup.setDuration(powerup.getDuration() - delta_time);

			if (powerup.getDuration() <= 0.0f) {
				// 此效果关闭使能
				powerup.setActivated(false);

				// 停用效果
				if (powerup.getType() == PowerUpType::STICKY) {
					if (!isOtherPowerupActive(_powerups, PowerUpType::STICKY)) {
						_ball->setSticky(false);
						_player->setColor(glm::vec3(1.0f));
					}
				} else if (powerup.getType() == PowerUpType::PASS_THROUNGH) {
					if (!isOtherPowerupActive(_powerups, PowerUpType::PASS_THROUNGH)) {
						_ball->setPassThrough(false);
						_ball->setColor(glm::vec3(1.0f));
					}
				} else if (powerup.getType() == PowerUpType::CONFUSE) {
					if (!isOtherPowerupActive(_powerups, PowerUpType::CONFUSE)) {
						_effect->setConfuse(false);
					}
				} else if (powerup.getType() == PowerUpType::CHAOS) {
					if (!isOtherPowerupActive(_powerups, PowerUpType::CHAOS)) {
						_effect->setChaos(false);
					}
				} 
			}
		}
	}

	// 擦除被销毁且停用的通电功能
	_powerups.erase(std::remove_if(_powerups.begin(), _powerups.end(),
		[](const PowerUp& powerup) {
			return powerup.getDestroyed() && !powerup.getActivated();
		}
	), _powerups.end());
}

inline void Breakout::disablePowerup() {
	for (PowerUp& powerup : _powerups) {
		if (powerup.getActivated()) {
			powerup.setDuration(0.0f);
			// 此效果关闭使能
			powerup.setActivated(false);
			// 停用效果
			if (powerup.getType() == PowerUpType::STICKY) {
				_ball->setSticky(false);
				_player->setColor(glm::vec3(1.0f));
			} else if (powerup.getType() == PowerUpType::PASS_THROUNGH) {
				_ball->setPassThrough(false);
				_ball->setColor(glm::vec3(1.0f));
			} else if (powerup.getType() == PowerUpType::CONFUSE) {
				_effect->setConfuse(false);
			} else if (powerup.getType() == PowerUpType::CHAOS) {
				_effect->setChaos(false);
			} 
		}
	}
}

inline void Breakout::clearPowerup() {
	disablePowerup();
	_powerups.clear();
}

inline GameState Breakout::getState() const {
	return _state;
}

inline void Breakout::setState(const GameState state) {
	_state = state;
}

inline unsigned int Breakout::getCheckpoint() const {
	return _checkpoint;
}

inline void Breakout::setCheckpoint(const unsigned int checkpoint) {
	_checkpoint = checkpoint;
}

inline unsigned int Breakout::getCheckpointsSize() {
	return static_cast<unsigned int>(_checkpoints.size());
}

inline std::array<bool, 1024>& Breakout::_getKeys() {
	return _keys;
}

inline void Breakout::init() {
	// 加载着色器
	// ----------
	ResourceManager::loadShader(ResourceManager::getPaths(PathType::SHADER).at("sprite_vs"), ResourceManager::getPaths(PathType::SHADER).at("sprite_fs"), NULL_STRING, "sprite");
	ResourceManager::loadShader(ResourceManager::getPaths(PathType::SHADER).at("particle_vs"), ResourceManager::getPaths(PathType::SHADER).at("particle_fs"), NULL_STRING, "particle");
	ResourceManager::loadShader(ResourceManager::getPaths(PathType::SHADER).at("post-processing_vs"), ResourceManager::getPaths(PathType::SHADER).at("post-processing_fs"), NULL_STRING, "post-processing");
	//
	// 2d 投影--始终在这里初始化投影矩阵
	glm:: mat4 projection(glm::ortho(0.0f, static_cast<float>(_width), static_cast<float>(_height), 0.0f, -1.0f, 1.0f));
	//
	// 1.1. 激活着色器程序; 1.2. 设置投影矩阵
	// 2. 设置纹理单元
	ResourceManager::_getShader("sprite").use().setMatrix4("projection", projection);
	ResourceManager::_getShader("sprite").setInteger("sprite", 0);
	ResourceManager::_getShader("particle").use().setMatrix4("projection", projection);
	ResourceManager::_getShader("particle").setInteger("sprite", 0);
	//
	// 加载纹理
	// 菜单背景
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("menu_bk"), true, "menu_bk");
	// 通用
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("bat_default"), true, "bat_default");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("ball_crystal"), false, "ball_crystal");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("ball_space-invader"), true, "ball_space-invader");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("square"), true, "ps");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("powerup_speed"), true, "powerup_speed");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("powerup_sticky"), true, "powerup_sticky");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("powerup_pass-through"), true, "powerup_pass-through");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("powerup_pad-size-increase"), true, "powerup_pad-size-increase");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("powerup_confuse"), true, "powerup_confuse");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("powerup_chaos"), true, "powerup_chaos");
	//
	// 关卡 1
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1bk1"), true, "1bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b1"), true, "1b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b2"), true, "1b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b3"), true, "1b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b4"), true, "1b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b5"), true, "1b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b6"), true, "1b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1b7"), true, "1b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("1s1"), true, "1s1");
	// 关卡 2
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2bk1"), false, "2bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b1"), true, "2b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b2"), true, "2b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b3"), true, "2b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b4"), true, "2b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b5"), true, "2b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b6"), true, "2b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2b7"), true, "2b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("2s1"), true, "2s1");
	// 关卡 3
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3bk1"), false, "3bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b1"), true, "3b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b2"), true, "3b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b3"), true, "3b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b4"), true, "3b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b5"), true, "3b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b6"), true, "3b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3b7"), true, "3b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("3s1"), true, "3s1");
	// 关卡 4
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4bk1"), false, "4bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b1"), true, "4b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b2"), true, "4b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b3"), true, "4b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b4"), true, "4b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b5"), true, "4b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b6"), true, "4b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4b7"), true, "4b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("4s1"), true, "4s1");
	// 关卡 5
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5bk1"), false, "5bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b1"), true, "5b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b2"), true, "5b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b3"), true, "5b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b4"), true, "5b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b5"), true, "5b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b6"), true, "5b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5b7"), true, "5b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("5s1"), true, "5s1");
	// 关卡 6
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6bk1"), false, "6bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b1"), true, "6b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b2"), true, "6b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b3"), true, "6b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b4"), true, "6b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b5"), true, "6b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b6"), true, "6b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6b7"), true, "6b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("6s1"), true, "6s1");
	// 关卡 7
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7bk1"), false, "7bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b1"), true, "7b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b2"), true, "7b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b3"), true, "7b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b4"), true, "7b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b5"), true, "7b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b6"), true, "7b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7b7"), true, "7b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("7s1"), true, "7s1");
	// 关卡 8
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8bk1"), false, "8bk1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b1"), true, "8b1");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b2"), true, "8b2");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b3"), true, "8b3");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b4"), true, "8b4");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b5"), true, "8b5");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b6"), true, "8b6");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8b7"), true, "8b7");
	ResourceManager::loadTexture2D(ResourceManager::getPaths(PathType::TEXTURE2D).at("8s1"), true, "8s1");
	//
	// 菜单瓦片
	_menu_bricks = std::make_unique<Checkpoint>();
	_menu_bricks->loadCheckpoint(0, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("menu")), _width, _height / 2, false);
	//
	// 加载级别
	Checkpoint cp1; cp1.loadCheckpoint(0, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("1")), _width, _height / 2, false);
	Checkpoint cp2; cp2.loadCheckpoint(1, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("2")), _width, _height / 2);
	Checkpoint cp3; cp3.loadCheckpoint(2, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("3")), _width, _height / 2);
	Checkpoint cp4; cp4.loadCheckpoint(3, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("4")), _width, _height / 2);
	Checkpoint cp5; cp5.loadCheckpoint(4, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("5")), _width, _height / 2);
	Checkpoint cp6; cp6.loadCheckpoint(5, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("6")), _width, _height / 2);
	Checkpoint cp7; cp7.loadCheckpoint(6, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("7")), _width, _height / 2);
	Checkpoint cp8; cp8.loadCheckpoint(7, ResourceManager::getCheckpoint(ResourceManager::getPaths(PathType::CHECKPOINT).at("8")), _width, _height / 2);
	_checkpoints.push_back(cp1);
	_checkpoints.push_back(cp2);
	_checkpoints.push_back(cp3);
	_checkpoints.push_back(cp4);
	_checkpoints.push_back(cp5);
	_checkpoints.push_back(cp6);
	_checkpoints.push_back(cp7);
	_checkpoints.push_back(cp8);
	//
	// 创建渲染器专用控件
	_renderer = std::make_unique<sprite::SpriteRenderer>(ResourceManager::_getShader("sprite"));
	_particles = std::make_unique<ParticleGenerator>(ResourceManager::_getShader("particle"), ResourceManager::_getTexture2D("ps"), 500);
	_effect = std::make_unique<sprite::PostProcessor>(ResourceManager::_getShader("post-processing"), _width, _height);
	//
	// 配置游戏对象--玩家
	glm::vec2 player_position((_width - _init->_PLAYER_SIZE.x) / 2.0f, _height - _init->_PLAYER_SIZE.y);
	_player = std::make_unique<GameObject>(player_position, _init->_PLAYER_SIZE, ResourceManager::_getTexture2D("bat_default"));
	//
	// 配置球对象
	glm::vec2 ball_position(player_position + glm::vec2(_init->_PLAYER_SIZE.x / 2.0f - _init->_BALL_RADIUS, -_init->_BALL_RADIUS * 2.0f));
	_ball = std::make_unique<Ball>(ball_position, _init->_BALL_RADIUS, _init->_INITIAL_BALL_VELOCITY, ResourceManager::_getTexture2D("ball_space-invader")/*, glm::vec3(0.0f, 1.0f, 1.0f)*/);
	// -----------------------------------------------------------------------------------------------

	// 游戏音频
	_audio_engine->init();
	_audio_engine->play2D(ResourceManager::getPaths(PathType::AUDIO).at("main_menu"), true, 0.86f);
}

}
