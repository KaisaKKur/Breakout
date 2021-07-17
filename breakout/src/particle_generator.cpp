#include "breakout/particle_generator.h"

namespace breakout {

Particle::Particle() : _position(0.0f), _velocity(0.0f), _color(1.0f), _life(0.0f) {
}

ParticleGenerator::ParticleGenerator(sprite::Shader& shader, const sprite::Texture2D& texture, unsigned int amount) : _last_use_particle(0), _shader(shader), _texture(texture), _amount(amount) {
	init();
}

ParticleGenerator::~ParticleGenerator() {
}

inline void ParticleGenerator::update(const float delta_time, GameObject& object, unsigned int new_particles, const glm::vec2& offset) {
	// 添加新粒子
	// ---------
	for (unsigned short i = 0; i < new_particles; i++) {
		int unused_particle = firstUnusedParticle();
		respawnParticle(_particles[unused_particle], object, offset);
	}
	// ---------------------------------------------------------------

	// 更新所有粒子
	// -----------
	for (unsigned short i = 0; i < _amount; i++) {
		Particle& p = _particles[i];
		p._life -= delta_time;
		if (p._life > 0.0f) {
			p._position -= p._velocity * delta_time;
			p._color.a	-= delta_time * 2.5f;
		}
	}
}

inline void ParticleGenerator::draw() {
	// 添加混合使其具有“发光”效果
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	_shader.use();

	for (const Particle& particle : _particles) {
		if (particle._life > 0.0f) {
			_shader.setVector2f("offset", particle._position);
			_shader.setVector4f("color", particle._color);
			glBindVertexArray(_vao);
			_texture.bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}
	}

	// 重置为默认混合模式
	glBlendFunc(GL_ONE, GL_ZERO);
}

inline void ParticleGenerator::init() {
	// 设置网格和属性
	// -------------
	std::vector<float> particles {
		// pos			// tex
		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f
	};
	std::vector<unsigned int> indices{
		0, 1, 3,
		1, 2, 3
	};
	//
	unsigned int vbo, ebo;
	//
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	//
	glBindVertexArray(_vao);
	//
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(float), particles.data(), GL_STATIC_DRAW);
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	//
	unsigned short enable_vertex_count = 0;
	//
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(enable_vertex_count++);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(enable_vertex_count++);
	//
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	for (unsigned short i = 0; i < enable_vertex_count; i++) {
		glDisableVertexAttribArray(i);
	}
	// --------------------------------------

	// 创建此数量的默认粒子实例
	// -----------------------
	for (unsigned short i = 0; i < _amount; i++) {
		_particles.push_back(Particle());
	}
	// ----------------------------------
}

inline unsigned int ParticleGenerator::firstUnusedParticle() {
	// 从最后使用过的粒子进行首次搜索，立即返回
	// -------------------------------------------
	for (unsigned int i = _last_use_particle; i < _amount; i++) {
		if (_particles[i]._life <= 0.0f) {
			_last_use_particle = i;
			return i;
		}
	}
	// --------------

	// 找不到，则进行线性搜索
	// ---------------------
	for (unsigned short i = 0; i < _last_use_particle; i++) {
		if (_particles[i]._life <= 0.0f) {
			_last_use_particle = i;
			return i;
		}
	}
	// ----------

	// 采集所有粒子，覆盖第一个粒子（请注意，如果反复遇到这种情况，则应保留更多粒子）
	// -------------------------------------------------------------------------
	_last_use_particle = 0;
	return 0;
	// ------
}

inline void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, const glm::vec2 offset) {
	std::random_device rd; // 将用于为随机数引擎获得种子
	std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
	std::uniform_real_distribution<> dis_pos(-50.0f, 50.0f); // 生成随机数
	std::uniform_real_distribution<> dis_clr(0.0f, 0.5f); // 生成随机数
	
	float random	= static_cast<float>(dis_pos(gen) / 10.0f);
	float color		= static_cast<float>(0.5f + dis_clr(gen));
	particle._position	= object.getPosition() + random + offset;
	particle._color		= glm::vec4(color, color, color, 1.0f);
	particle._life		= 1.0f;
	particle._velocity	= object.getVelocity() * 0.1f;
}

}
