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

bool IsCollision(const Segment& segment, const Plane& plane) {
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直の時は衝突していないのでfalseを返す
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(plane.normal, segment.origin)) / dot;

	if (t >= 0.0f && t <= 1.0f) {
		return true;
	} else {
		return false;
	}
}