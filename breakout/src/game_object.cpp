#include "breakout/game_object.h"

namespace breakout {

GameObject::GameObject() : _position(0.0f, 0.0f), _size(1.0f, 1.0f), _velocity(0.0f), _color(1.0f), _rotation(0.0f), _sprite(), _solid(false), _destroyed(false) {
}

GameObject::GameObject(const glm::vec2& position, const glm::vec2& size, const sprite::Texture2D& sprite, const bool blend_alpha, const glm::vec3& color, const glm::vec2& velocity) : _position(position), _size(size), _velocity(velocity), _color(color), _rotation(0.0f), _sprite(sprite), _solid(false), _destroyed(false), _blend_alpha(blend_alpha) {
}

GameObject::~GameObject() {
}

inline void GameObject::draw(sprite::SpriteRenderer& renderer) {
	if (!getBlendAlpha()) {
		glBlendFunc(GL_ONE, GL_ZERO);
		renderer.drawSprite(_sprite, _position, _size, _rotation, _color);
	} else {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		renderer.drawSprite(_sprite, _position, _size, _rotation, _color);
		// 重置为默认混合模式
		glBlendFunc(GL_ONE, GL_ZERO);
	}
}

inline bool GameObject::getDestroyed() const {
	return _destroyed;
}

inline glm::vec2 GameObject::getPosition() const {
	return _position;
}

inline void GameObject::setPosition(const glm::vec2& position) {
	_position = position;
}

inline glm::vec2 GameObject::getSize() const {
	return _size;
}

inline void GameObject::setSize(const glm::vec2& size) {
	_size = size;
}

inline glm::vec2 GameObject::getVelocity() const {
	return _velocity;
}

inline void GameObject::setVelocity(const glm::vec2& velocity) {
	_velocity = velocity;
}

inline glm::vec3 GameObject::getColor() const {
	return _color;
}

inline void GameObject::setColor(const glm::vec3& color) {
	_color = color;
}

inline bool GameObject::getSolid() const {
	return _solid;
}

inline void GameObject::setDestroyed(const bool destroyed) {
	_destroyed = destroyed;
}

inline bool GameObject::getBlendAlpha() const {
	return _blend_alpha;
}

inline void GameObject::setBlendAlpha(const bool blend_alpha) {
	_blend_alpha = blend_alpha;
}

inline glm::vec2& GameObject::_getPosition() {
	return _position;
}

inline glm::vec2& GameObject::_getSize() {
	return _size;
}

inline glm::vec2& GameObject::_getVelocity() {
	return _velocity;
}

inline void GameObject::setSolid(const bool solid) {
	_solid = solid;
}

}
