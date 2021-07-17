#include "sprite/texture2d.h"

namespace sprite {

Texture2D::Texture2D() : _width(0), _height(0), _internal_format(GL_RGB), _image_format(GL_RGB), _wrap_s(GL_REPEAT), _wrap_t(GL_REPEAT), _filter_min(GL_LINEAR), _filter_max(GL_LINEAR) {
	glGenTextures(1, &_id);
}

inline unsigned int Texture2D::getId() const {
	return _id;
}

inline void Texture2D::setInternalFormat(unsigned int alpha) {
	_internal_format = alpha;
}

inline void Texture2D::setImageFormat(unsigned int alpha) {
	_image_format = alpha;
}

inline unsigned int& Texture2D::_getId() {
	return _id;
}

inline void Texture2D::generate(const unsigned int width, const unsigned int height, const unsigned char* data) {
	// >...
	_width = width;
	_height = height;
	//
	// >...
	glBindTexture(GL_TEXTURE_2D, _id);
	//
	// >...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_t);
	//
	// >...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter_max);
	//
	// >...
	glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, _image_format, GL_UNSIGNED_BYTE, data);
	// 
	// 清理资源
	glBindTexture(GL_TEXTURE_2D, 0);
	// -----------------------------
}

inline void Texture2D::bind() const {
	glBindTexture(GL_TEXTURE_2D, _id);
}

}
