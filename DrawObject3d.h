#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include<iostream>

struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

struct Line {
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

struct Ray {
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

struct Segment {
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

struct Triangle {
	Vector3 vertices[3]; // 頂点
};

struct Sphere {
	Vector3 center; // 中心
	float radius;   // 半径
};

struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

struct Box {
	Vector3 vertices[8]; // 頂点
};

namespace DrawObject3D {

	// グリッドを描画
	void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	// 球を描画
	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	// AABBの描画
	void DrawAABB(const AABB aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
}
