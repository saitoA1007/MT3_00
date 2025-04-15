#include"DrawObject3d.h"
#include"Math.h"
#include<cmath>
#include <stdint.h>
#include<Novice.h>
#include<vector>

static const double M_PI = 3.14159265358979323846;

// グリッドを描画
void DrawObject3D::DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	// Gridの描画で使う変数
	Vector3 start;
	Vector3 end;
	Vector3 startScreen;
	Vector3 endScreen;
	Vector3 ndcStart;
	Vector3 ndcEnd;

	// グリッドの半分の長さ
	const float kGridHalfWidth = 2.0f;
	// 分割数
	const uint32_t kSubdivision = 10;
	// 1つ分の長さ
	const float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivision);

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// ワールド座標上の始点と終点を求める
		start = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * static_cast<float>(xIndex) };
		end = { kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * static_cast<float>(xIndex) };
		// ndcまで変換
		ndcStart = Transform(start, viewProjectionMatrix);
		ndcEnd = Transform(end, viewProjectionMatrix);
		// スクリーン座標系まで変換をかける
		startScreen = Transform(ndcStart, viewportMatrix);
		endScreen = Transform(ndcEnd, viewportMatrix);

		// Gridを描画
		if (xIndex == kSubdivision / 2) {
			// 原点の線
			Novice::DrawLine(static_cast<int>(startScreen.x), static_cast<int>(startScreen.y), static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0x000000FF);
		} else {
			// 灰色の線
			Novice::DrawLine(static_cast<int>(startScreen.x), static_cast<int>(startScreen.y), static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
		}
	}

	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// ワールド座標上の始点と終点を求める
		start = { -kGridHalfWidth + kGridEvery * static_cast<float>(zIndex), 0.0f, -kGridHalfWidth };
		end = { -kGridHalfWidth + kGridEvery * static_cast<float>(zIndex), 0.0f, kGridHalfWidth };
		// ndcまで変換
		ndcStart = Transform(start, viewProjectionMatrix);
		ndcEnd = Transform(end, viewProjectionMatrix);
		// スクリーン座標系まで変換をかける
		startScreen = Transform(ndcStart, viewportMatrix);
		endScreen = Transform(ndcEnd, viewportMatrix);

		// Gridを描画
		if (zIndex == kSubdivision / 2) {
			// 原点の線
			Novice::DrawLine(static_cast<int>(startScreen.x), static_cast<int>(startScreen.y), static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0x000000FF);
		} else {
			// 灰色の線
			Novice::DrawLine(static_cast<int>(startScreen.x), static_cast<int>(startScreen.y), static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
		}
	}
}

void DrawObject3D::DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.normal, plane.distance); // 中心点を決める
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal)); // 法線と垂直なベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z }; // 逆ベクトルを求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]); // 法線とのクロス積を求める
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z }; // 逆ベクトルを求める
	// 各ベクトルに中心点にそれぞれ定数倍して足し、頂点を作る
	Vector3 points[4];
	for (int32_t i = 0; i < 4; ++i) {
		Vector3 extend = Multiply(perpendiculars[i],2.0f);
		Vector3 point = center + extend;
		points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	//// pointsを結び矩形を描画
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[3].x), static_cast<int>(points[3].y), static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
}

void DrawObject3D::DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// 分割数
	const uint32_t kSubdivision = 10;
	// 緯度分割1つ分の角度
	const float kLatEvery = static_cast<float>(M_PI) / kSubdivision;
	// 経度分割1つ分の角度
	const float kLonEvery = 2.0f * static_cast<float>(M_PI) / kSubdivision;

	// WVPMatrixを作成
	Matrix4x4 worldViewProjectionMatrix = Multiply(MakeTranslateMatrix(sphere.center), viewProjectionMatrix);

	// 緯度の方向に分割　-π/2からπまで
	for (uint32_t latIndex = 0; latIndex < kSubdivision * 2; ++latIndex) {
		// 現在の緯度
		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * static_cast<float>(latIndex);

		// 経度の方向に分割　0から2πまで
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			// 現在の経度
			float lon = lonIndex * kLonEvery;

			// world座標系でのa,b,cを求める
			Vector3 a = { sphere.radius * cosf(lat) * cosf(lon),sphere.radius * sinf(lat),sphere.radius * cosf(lat) * sinf(lon) };
			Vector3 b = { sphere.radius * cosf(lat + kLatEvery) * cosf(lon),sphere.radius * sinf(lat + kLatEvery),sphere.radius * cosf(lat + kLatEvery) * sinf(lon) };
			Vector3 c = { sphere.radius * cosf(lat) * cosf(lon + kLonEvery),sphere.radius * sinf(lat),sphere.radius * cosf(lat) * sinf(lon + kLonEvery) };
			// NDCまで変換
			Vector3 ndcA = Transform(a, worldViewProjectionMatrix);
			Vector3 ndcB = Transform(b, worldViewProjectionMatrix);
			Vector3 ndcC = Transform(c, worldViewProjectionMatrix);
			// スクリーン座標系に変換
			Vector3 screenA = Transform(ndcA, viewportMatrix);
			Vector3 screenB = Transform(ndcB, viewportMatrix);
			Vector3 screenC = Transform(ndcC, viewportMatrix);

			// ab,acで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}

void DrawObject3D::DrawAABB(const AABB aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Box box;

	// 各頂点に変換
	box = {
		aabb.min,
		Vector3(aabb.min.x, aabb.min.y, aabb.max.z),
		Vector3(aabb.min.x, aabb.max.y, aabb.min.z),
		Vector3(aabb.min.x, aabb.max.y, aabb.max.z),
		Vector3(aabb.max.x, aabb.min.y, aabb.min.z),
		Vector3(aabb.max.x, aabb.min.y, aabb.max.z),
		Vector3(aabb.max.x, aabb.max.y, aabb.min.z),
		aabb.max
	};

	// スクリーン座標に必要な変数
	Box ndcBox;
	Box screenBox;

	// スクリーン座標に変換
	for (int i = 0; i < 8; ++i) {
		ndcBox.vertices[i] = Transform(box.vertices[i], viewProjectionMatrix);
		screenBox.vertices[i] = Transform(ndcBox.vertices[i], viewportMatrix);
	}

	// AABBのエッジの接続順番
	std::vector<std::pair<int, int>> edges = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 底面の4辺
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 上面の4辺
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 上下をつなぐ4辺
	};

	// 各エッジを描画
	for (const auto& edge : edges) {
		Vector3 startPos = screenBox.vertices[edge.first];
		Vector3 endPos = screenBox.vertices[edge.second];

		// 線を描画
		Novice::DrawLine(static_cast<int>(startPos.x), static_cast<int>(startPos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y), color);
	}
}
