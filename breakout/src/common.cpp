#include "breakout/common.h"

namespace breakout {

// function
// --------
bool checkCollision(GameObject& one, GameObject& two) {
	/* X 轴碰撞 ? */
	bool collision_x = one.getPosition().x + one.getSize().x >= two.getPosition().x && two.getPosition().x + two.getSize().x >= one.getPosition().x;
	/* Y 轴碰撞 ? */
	bool collision_y = one.getPosition().y + one.getSize().y >= two.getPosition().y && two.getPosition().y + two.getSize().y >= one.getPosition().y;
	/* 仅在两个轴上都发生碰撞 */
	return collision_x && collision_y;
}

collision checkCollision(Ball& one, GameObject& two) {
	/* 首先获得圆的中心 */
	glm::vec2 circle_center(one.getPosition() + one.getRadius());
	/* 计算 AABB 信息（中心，半范围） */
	glm::vec2 aabb_half_extents(two.getSize().x / 2.0f, two.getSize().y / 2.0f);
	glm::vec2 aabb_center(two.getPosition().x + aabb_half_extents.x, two.getPosition().y + aabb_half_extents.y);
	/* 得到两个中心之间的差向量 */
	glm::vec2 difference = circle_center - aabb_center;
	glm::vec2 clamped(glm::clamp(difference, -aabb_half_extents, aabb_half_extents));
	/* 将限定值添加到 AABBcenter，我们得到最接近圆的框的值 */
	glm::vec2 closest = aabb_center + clamped;
	/* 检索圆的中心和 AABB 之间最近的点的向量，并检查长度是否 <= 半径 */
	difference = closest - circle_center;
	if (glm::length(difference) <= one.getRadius()) {
		return std::make_tuple(true, vectorDirection(difference), difference);
	} else {
		return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
	}
}

/* 点积求反弹方向 */
Direction vectorDirection(const glm::vec2& target) {
	std::array<glm::vec2, 4> compass {
		glm::vec2( 0.0f,	1.0f),
		glm::vec2( 1.0f,	0.0f),
		glm::vec2( 0.0f,   -1.0f),
		glm::vec2(-1.0f,	0.0f)
	};
	float max = 0.0f, dot_product = 0.0f;
	int best_match = -1;
	for (unsigned short i = 0; i < 4; i++) {
		dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return static_cast<Direction>(best_match);
}
// -----------------------------------

}
