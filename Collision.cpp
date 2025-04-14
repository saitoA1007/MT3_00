#include"Collision.h"
#include"Math.h"

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	// 半径の合計より短ければ衝突
	if (Length(Subtract(s1.center, s2.center)) < s1.radius + s2.radius) {
		return true;
	} else {
		return false;
	}
}