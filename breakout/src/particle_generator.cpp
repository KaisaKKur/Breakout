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
	// ���������
	// ---------
	for (unsigned short i = 0; i < new_particles; i++) {
		int unused_particle = firstUnusedParticle();
		respawnParticle(_particles[unused_particle], object, offset);
	}
	// ---------------------------------------------------------------

	// ������������
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
	// ��ӻ��ʹ����С����⡱Ч��
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

	// ����ΪĬ�ϻ��ģʽ
	glBlendFunc(GL_ONE, GL_ZERO);
}

inline void ParticleGenerator::init() {
	// �������������
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

	// ������������Ĭ������ʵ��
	// -----------------------
	for (unsigned short i = 0; i < _amount; i++) {
		_particles.push_back(Particle());
	}
	// ----------------------------------
}

inline unsigned int ParticleGenerator::firstUnusedParticle() {
	// �����ʹ�ù������ӽ����״���������������
	// -------------------------------------------
	for (unsigned int i = _last_use_particle; i < _amount; i++) {
		if (_particles[i]._life <= 0.0f) {
			_last_use_particle = i;
			return i;
		}
	}
	// --------------

	// �Ҳ������������������
	// ---------------------
	for (unsigned short i = 0; i < _last_use_particle; i++) {
		if (_particles[i]._life <= 0.0f) {
			_last_use_particle = i;
			return i;
		}
	}
	// ----------

	// �ɼ��������ӣ����ǵ�һ�����ӣ���ע�⣬����������������������Ӧ�����������ӣ�
	// -------------------------------------------------------------------------
	_last_use_particle = 0;
	return 0;
	// ------
}

inline void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, const glm::vec2 offset) {
	std::random_device rd; // ������Ϊ���������������
	std::mt19937 gen(rd()); // �Բ��ֱ�׼ mersenne_twister_engine
	std::uniform_real_distribution<> dis_pos(-50.0f, 50.0f); // ���������
	std::uniform_real_distribution<> dis_clr(0.0f, 0.5f); // ���������
	
	float random	= static_cast<float>(dis_pos(gen) / 10.0f);
	float color		= static_cast<float>(0.5f + dis_clr(gen));
	particle._position	= object.getPosition() + random + offset;
	particle._color		= glm::vec4(color, color, color, 1.0f);
	particle._life		= 1.0f;
	particle._velocity	= object.getVelocity() * 0.1f;
}

}
