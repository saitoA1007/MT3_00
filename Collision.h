#pragma once
#include"Geometry.h"

// 球同士の衝突判定
bool IsSpheresCollision(const Sphere& s1, const Sphere& s2);

// 平面と球の衝突判定
bool IsSpherePlaneCollision(const Sphere& sphere, const Plane& plane);

// 線と平面の衝突判定
bool IsSegmentPlaneCollision(const Segment& segment, const Plane& plane);

// 線と三角形の衝突判定
bool IsSegmentTriangleCollision(const Triangle& triangle, const Segment& segment);