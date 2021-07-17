#include "breakout/ball.h"

namespace breakout {

Ball::Ball() : GameObject(), _radius(12.5f), _stuck(true), _sticky(false), _pass_through(false) {
}

Ball::Ball(const glm::vec2& position, float radius, const glm::vec2& velocity, const sprite::Texture2D& sprite, const glm::vec3& color) : GameObject(position, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, true, color, velocity), _radius(radius), _stuck(true), _sticky(false), _pass_through(false) {
}

Ball::~Ball() {
}

inline glm::vec2 Ball::move(const float delta_time, const unsigned int window) {
	// �����ճ����Ұ���, ��ȷ�ƶ���
	// -----------------------------
	if (!_stuck) {
		_position += _velocity * delta_time; /* �ƶ��� */
		//
		// Ȼ�����Ƿ��ڴ��ڱ߽�֮�⣬����ǣ���ת�ٶȲ��ָ�����ȷ��λ��
		// x
		if (_position.x <= 0.0f) {
			_velocity.x = -_velocity.x;
			_position.x = 0.0f;
		} else if (_position.x + _size.x >= window) {
			_velocity.x = -_velocity.x;
			_position.x = window - _size.x;
		}
		//
		// y
		if (_position.y <= 0.0f) {
			_velocity.y = -_velocity.y;
			_position.y = 0.0f;
		}
	}
	//
	// >***
	return _position;
	// --------------
}

void Ball::reset(const glm::vec2& position, const glm::vec2& velocity) {
	_position	= position;
	_velocity	= velocity;
	_stuck		= true;
}

inline float Ball::getRadius() const {
	return _radius;
}

inline bool Ball::getStuck() const {
	return _stuck;
}

inline void Ball::setStuck(const bool stuck) {
	_stuck = stuck;
}

inline bool Ball::getSticky() const {
	return _sticky;
}

inline void Ball::setSticky(const bool sticky) {
	_sticky = sticky;
}

inline bool Ball::getPassThrough() const {
	return _pass_through;
}

inline void Ball::setPassThrough(const bool pass_through) {
	_pass_through = pass_through;
}

}
