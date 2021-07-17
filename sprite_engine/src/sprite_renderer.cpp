#include "sprite/sprite_renderer.h"

namespace sprite {

SpriteRenderer::SpriteRenderer(Shader& shader) : _shader(shader) {
	initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &_vao);
}

inline void SpriteRenderer::drawSprite(const Texture2D& Texture2D, const glm::vec2& position, const glm::vec2& size, const float rotate, const glm::vec3& color) {
	_shader.use();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));  /* 首先平移（转换是：先发生缩放，然后旋转，然后发生最终平移；取反 */

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); /* 将旋转原点移到四边形的中心 */
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); /* 然后旋转 */
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); /* 移回原点 */

	model = glm::scale(model, glm::vec3(size, 1.0f)); /* 最后缩放 */

	_shader.setMatrix4("model", model);
	_shader.setVector3f("sprite_color", color);

	glBindVertexArray(_vao);
	glActiveTexture(GL_TEXTURE0);
	Texture2D.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

inline void SpriteRenderer::initRenderData() {
	std::vector<float> vertices{
		// position			// texture
		0.0f, 1.0f,			0.0f, 1.0f, // top left
		1.0f, 1.0f,			1.0f, 1.0f, // top right
		1.0f, 0.0f,			1.0f, 0.0f, // bottom right
		0.0f, 0.0f,			0.0f, 0.0f  // bottom left 
	};
	std::vector<unsigned int> indices{
		0, 1, 3,
		1, 2, 3
	};

	// 缓冲区设置
	// ----------
	// >...
	unsigned int vbo, ebo;
	//
	// >...
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	//
	// 顶点数组对象
	glBindVertexArray(_vao);
	//
	// 顶点缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	//
	// 元素缓冲对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	// -----------------------------------------------------------------------------------

	// 顶点属性设置
	// -----------
	// 位置
	unsigned short enable_vertex_count = 0;
	//
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(enable_vertex_count++);
	//
	// 纹理
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(enable_vertex_count++);
	// --------------------------

	// 清理资源
	// --------
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	for (unsigned short i = 0; i < enable_vertex_count; i++) {
		glDisableVertexAttribArray(i);
	}
	// ------------------
}

}
