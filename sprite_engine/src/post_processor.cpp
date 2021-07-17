#include "sprite/post_processor.h"

namespace sprite {

PostProcessor::PostProcessor(sprite::Shader& shader, const unsigned int width, const unsigned int height) : _shader(shader), _texture(), _width(width), _height(height), _chaos(false), _confuse(false), _shake(false), _effect_times(3) {
	try {
		glGenFramebuffers(1, &_msfbo);
		glGenFramebuffers(1, &_fbo);
		glGenRenderbuffers(1, &_rbo);
		// 使用多采样颜色缓冲区初始化渲染缓冲区存储（不需要深度/模板缓冲区）
		glBindFramebuffer(GL_FRAMEBUFFER, _msfbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rbo);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::exception("ERROR::POSTPROCESSOR: Failed to initialize MSFBO");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		_texture.generate(width, height, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture.getId(), 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::exception("ERROR::POSTPROCESSOR: Failed to initialize FBO");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
	// 初始化渲染数据和制服
	initRenderData();
	_shader.setInteger("scene", 0, true);
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	glUniform2fv(glGetUniformLocation(_shader.getId(), "offsets"), 9, reinterpret_cast<float*>(offsets));
	
	std::vector<int> edges {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(_shader.getId(), "edges"), 9, edges.data());

	std::vector<float> blurs {
		1.0f / 16.0f,	2.0f / 16.0f,	1.0f / 16.0f,
		2.0f / 16.0f,	4.0f / 16.0f,	2.0f / 16.0f,
		1.0f / 16.0f,	2.0f / 16.0f,	1.0f / 16.0f
	};
	glUniform1fv(glGetUniformLocation(_shader.getId(), "blurs"), 9, blurs.data());
}

inline void PostProcessor::beginRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, _msfbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

inline void PostProcessor::endRender() {
	// 将多重采样的颜色缓冲区解析为中间FBO以存储为纹理
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _msfbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	// 清理资源
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void PostProcessor::render(const float time) {
	_shader.use();
	_shader.setFloat("time", time);
	_shader.setInteger("chaos", _chaos);
	_shader.setInteger("confuse", _confuse);
	_shader.setInteger("shake", _shake);

	glBindVertexArray(_vao);
	glActiveTexture(GL_TEXTURE0);
	_texture.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

inline void PostProcessor::reset() {
	_chaos = false;
	_confuse = false;
	_shake = false;
	for (auto& time : _effect_times) {
		time = 0;
	}
}

inline void PostProcessor::initRenderData() {
	std::vector<float> vertices {
		// pos				// tex
		-1.0f,   1.0f,		0.0f, 1.0f,
		 1.0f,   1.0f,		1.0f, 1.0f,
		 1.0f,  -1.0f,		1.0f, 0.0f,
		-1.0f,  -1.0f,		0.0f, 0.0f,
	};
	std::vector<unsigned int> indices{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int vbo, ebo;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	unsigned short enable_vertex_count = 0;

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(enable_vertex_count++);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(enable_vertex_count++);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	for (unsigned short i = 0; i < enable_vertex_count; i++) {
		glDisableVertexAttribArray(i);
	}
}

inline void PostProcessor::setShake(bool shake) {
	_shake = shake;
}

inline bool PostProcessor::getConfuse() const {
	return _confuse;
}

inline void PostProcessor::setConfuse(const bool confuse) {
	_confuse = confuse;
}

inline bool PostProcessor::getChaos() const {
	return _chaos;
}

inline void PostProcessor::setChaos(const bool chaos) {
	_chaos = chaos;
}

inline float PostProcessor::getShakeEffectTime() const {
	return _effect_times[2];
}

inline void PostProcessor::setShakeEffectTime(const float effect_time) {
	_effect_times[2] = effect_time;
}

}
