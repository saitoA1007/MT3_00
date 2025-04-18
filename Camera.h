#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include <stdint.h>
#include"InPutProcess.h"

class Camera {
public:

	// コンストラクト
	Camera(const Vector3& scale, const Vector3& rotate, const Vector3& cameraPos,const int kWindowWidth, const int kWindowHeight);

	/// <summary>
	/// カメラ座標を決める
	/// </summary>
	/// <param name="cameraPos">カメラ座標</param>
	void TransCameraPos(Vector3 cameraPos);

	/// <summary>
	/// スクリーン座標に変換
	/// </summary>
	/// <param name="localPos">ローカル座標</param>
	/// <param name="worldMatrix">ワールド行列</param>
	/// <returns></returns>
	Vector3 TransScreen(Vector3 localPos, Matrix4x4 worldMatrix);

	/// <summary>
	/// スクリーン座標に変換
	/// </summary>
	/// <param name="localPos">ワールド座標</param>
	/// <returns></returns>
	Vector3 TransScreen(Vector3 worldPos);

	// VPMatrixのゲッター
	Matrix4x4 GetViewProjectionMatrix();

	// ビューポート行列のゲッター
	Matrix4x4 GetViewportMatrix();

#ifdef _DEBUG

	// カメラのデバック
	void DrawCameraDebugWindow(InPut& input);

#endif 

private:

	// ワールド座標上のカメラ座標
	Vector3 cameraPos_;
	Matrix4x4 transMatrix_;
	// カメラの角度
	Vector3 cameraRotate_;
	Matrix4x4 rotateMatrix_;
	// カメラの拡縮
	Vector3 cameraScale_;
	Matrix4x4 scaleMatrix_;
	// カメラのワールド座標上の行列
	Matrix4x4 cameraWorldMatrix_;
	// ビュー行列
	Matrix4x4 viewMatrix_;
	// 透視投影行列
	Matrix4x4 projectionMatrix_;
	// WVPMatrix
	Matrix4x4 worldViewProjectionMatrix_;
	// ビューポート行列
	Matrix4x4 viewportMatrix_;
	// NDCまで変換
	Vector3 ndcVertex_;
	// VPMatrix
	Matrix4x4 viewProjectionMatrix_;
};
