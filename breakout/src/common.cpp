#include "breakout/common.h"

namespace breakout {

// function
// --------
bool checkCollision(GameObject& one, GameObject& two) {
	/* X ����ײ ? */
	bool collision_x = one.getPosition().x + one.getSize().x >= two.getPosition().x && two.getPosition().x + two.getSize().x >= one.getPosition().x;
	/* Y ����ײ ? */
	bool collision_y = one.getPosition().y + one.getSize().y >= two.getPosition().y && two.getPosition().y + two.getSize().y >= one.getPosition().y;
	/* �����������϶�������ײ */
	return collision_x && collision_y;
}

collision checkCollision(Ball& one, GameObject& two) {
	/* ���Ȼ��Բ������ */
	glm::vec2 circle_center(one.getPosition() + one.getRadius());
	/* ���� AABB ��Ϣ�����ģ��뷶Χ�� */
	glm::vec2 aabb_half_extents(two.getSize().x / 2.0f, two.getSize().y / 2.0f);
	glm::vec2 aabb_center(two.getPosition().x + aabb_half_extents.x, two.getPosition().y + aabb_half_extents.y);
	/* �õ���������֮��Ĳ����� */
	glm::vec2 difference = circle_center - aabb_center;
	glm::vec2 clamped(glm::clamp(difference, -aabb_half_extents, aabb_half_extents));
	/* ���޶�ֵ��ӵ� AABBcenter�����ǵõ���ӽ�Բ�Ŀ��ֵ */
	glm::vec2 closest = aabb_center + clamped;
	/* ����Բ�����ĺ� AABB ֮������ĵ������������鳤���Ƿ� <= �뾶 */
	difference = closest - circle_center;
	if (glm::length(difference) <= one.getRadius()) {
		return std::make_tuple(true, vectorDirection(difference), difference);
	} else {
		return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
	}
}

/* ����󷴵����� */
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
