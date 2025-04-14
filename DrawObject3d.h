#pragma once
#include"Geometry.h"
#include"Matrix4x4.h"
#include<iostream>

namespace DrawObject3D {

	// グリッドを描画
	void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	// 球を描画
	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	// AABBの描画
	void DrawAABB(const AABB aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
}
