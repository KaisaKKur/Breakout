#include "breakout/power_up.h"

namespace breakout {

PowerUp::PowerUp(const PowerUpType& type, const glm::vec3& color, float duration, const glm::vec2& position, const sprite::Texture2D& texture) : GameObject(position, glm::vec2(60.0f, 20.0f), texture, false, color, glm::vec2(0.0f, 150.0f)), _type(type), _duration(duration), _activated() {
}

inline PowerUpType PowerUp::getType() const {
	return _type;
}

inline float PowerUp::getDuration() const {
	return _duration;
}

inline void PowerUp::setDuration(const float duration) {
	_duration = duration;
}

inline bool PowerUp::getActivated() const {
	return _activated;
}

inline void PowerUp::setActivated(const bool activated) {
	_activated = activated;
}

}
