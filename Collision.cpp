#include"Collision.h"
#include"Math.h"
#include<cmath>

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	// 半径の合計より短ければ衝突
	if (Length(Subtract(s1.center, s2.center)) < s1.radius + s2.radius) {
		return true;
	} else {
		return false;
	}
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {
	// 球の半径より短ければ衝突
	if (std::fabs(Dot(plane.normal, sphere.center) - plane.distance) <= sphere.radius) {
		return true;
	} else {
		return false;
	}
}