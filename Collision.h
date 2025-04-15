#pragma once
#include"Geometry.h"

// 球同士の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

// 平面と球の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

// 線と平面の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane);