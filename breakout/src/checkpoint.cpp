#include "breakout/checkpoint.h"

namespace breakout {

Checkpoint::Checkpoint() : _bricks(), _count(0), _tick(0) {
}

inline void Checkpoint::loadCheckpoint(const unsigned int checkpoint, const std::vector<std::vector<int>>& tile_data, const unsigned int width, const unsigned int height, const bool blend_alpha) {
	// 清除旧数据
	// ----------
	_bricks.clear();
	_count = 0;
	_tick = 0;
	// -------------

	if (!tile_data.empty()) {
		init(checkpoint, blend_alpha, tile_data, width, height);
	}
	// ------------------------------------
}

inline void Checkpoint::draw(sprite::SpriteRenderer& renderer) {
	for (GameObject& tile : _bricks) {
		if (!tile.getDestroyed()) {
			tile.draw(renderer);
		}
	}
}

inline bool Checkpoint::isCompleted() {
	if (_tick == 0) {
		resetTick();
		return true;
	} else {
		return false;
	}
}

inline void Checkpoint::runTick() {
	_tick--;
}

inline void Checkpoint::resetTick() {
	_tick = _count;
}

inline void Checkpoint::init(const unsigned int checkpoint, const bool blend_alpha, const std::vector<std::vector<int>>& tile_data, const unsigned int width, const unsigned int height) {
	// 配置关卡
	std::string cp = std::to_string(checkpoint + 1);
	// 计算尺寸
	// --------
	unsigned int h	= static_cast<unsigned int>(tile_data.size());
	unsigned int w	= static_cast<unsigned int>(tile_data[0].size());
	float unit_width	= width / static_cast<float>(w);
	float unit_height	= height / static_cast<float>(h);
	// -------------------------------------------------------------

	// 根据tileData初始化关卡
	// ---------------------
	for (unsigned short y = 0; y < h; y++) {
		for (unsigned short x = 0; x < w; x++) {
			/* 从级别数据中检查块类型（二维级别数组） */
			if (tile_data[y][x] == -1) { /* 实心砖，不能破坏的砖 */
				glm::vec2 position(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				glm::vec3 color(0.8f, 0.8f, 0.7f);
				GameObject obj(position, size, ResourceManager::_getTexture2D(cp + "s1"), blend_alpha, color);
				obj.setSolid(true);
				_bricks.push_back(obj);
			} else if (tile_data[y][x] > 0) { /* 非实心砖, 现在根据级别数据确定其具颜色瓦片 */
				glm::vec2 position(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				if (tile_data[y][x] == 1) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b1"), blend_alpha));
				} else if (tile_data[y][x] == 2) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b2"), blend_alpha));
				} else if (tile_data[y][x] == 3) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b3"), blend_alpha));
				} else if (tile_data[y][x] == 4) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b4"), blend_alpha));
				} else if (tile_data[y][x] == 5) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b5"), blend_alpha));
				} else if (tile_data[y][x] == 6) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b6"), blend_alpha));
				} else if (tile_data[y][x] == 7) {
					_bricks.push_back(GameObject(position, size, ResourceManager::_getTexture2D(cp + "b7"), blend_alpha));
				}
				_count++;
			}
		}
	}
	_tick = _count;
	// -------------------------------------------------------------------------------------------------------
}

inline std::vector<GameObject> Checkpoint::getBricks() const {
	return _bricks;
}

inline unsigned int Checkpoint::getCount() const {
	return _count;
}

inline unsigned int Checkpoint::getTick() const {
	return _tick;
}

inline void Checkpoint::setTick(const unsigned int tick) {
	_tick = tick;
}

inline std::vector<GameObject>& Checkpoint::_getBricks() {
	return _bricks;
}

}
