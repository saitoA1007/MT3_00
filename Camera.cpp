#include"Camera.h"
#include"Math.h"
#include<Novice.h>
#include <stdint.h>
#include<imgui.h>

static const double M_PI = 3.14159265358979323846;

Camera::Camera(const Vector3& scale,const Vector3& rotate, const Vector3& cameraPos, const int kWindowWidth, const int kWindowHeight) {
	// カメラ座標の初期化
	cameraScale_ = scale;
	cameraRotate_ = rotate;
	cameraPos_ = cameraPos;
	cameraWorldMatrix_ = MakeAffineMatrix(scale, rotate, cameraPos);
	// ビュー行列の初期化
	viewMatrix_ = Inverse(cameraWorldMatrix_);
	// 透視投影行列の初期化
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
	// ビューポート行列の初期化
	viewportMatrix_ = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

	// VPMatrixを作る
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::TransCameraPos(Vector3 cameraPos) {
	cameraPos_ = cameraPos;
	cameraWorldMatrix_ = MakeTranslateMatrix(cameraPos_);
	// ビュー行列の変更
	viewMatrix_ = Inverse(cameraWorldMatrix_);

	// VPMatrixを作る
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

Vector3 Camera::TransScreen(Vector3 localPos, Matrix4x4 worldMatrix) {
	Vector3 screenPos;
	// WVPMatrixを作る
	worldViewProjectionMatrix_ = Multiply(worldMatrix, Multiply(viewMatrix_, projectionMatrix_));
	// NDCまで変換
	ndcVertex_ = Transform(localPos, worldViewProjectionMatrix_);
	// スクリーン座標に変換
	screenPos = Transform(ndcVertex_, viewportMatrix_);
	return screenPos;
}

Vector3 Camera::TransScreen(Vector3 worldPos) {
	Vector3 screenPos;
	// NDCまで変換
	ndcVertex_ = Transform(worldPos, viewProjectionMatrix_);
	// スクリーン座標に変換
	screenPos = Transform(ndcVertex_, viewportMatrix_);
	return screenPos;
}

#ifdef _DEBUG

void Camera::DrawCameraDebugWindow() {
	// カメラのデバック
	ImGui::Begin("DebugCameraWindow");
	ImGui::DragFloat3("cameraPos", &cameraPos_.x, 0.01f);
	ImGui::DragFloat3("cameraRotate", &cameraRotate_.x, 0.01f);
	ImGui::DragFloat3("cameraScale", &cameraScale_.x, 0.01f);
	ImGui::End();

	// カメラの変更した内容を適用する処理
	cameraWorldMatrix_ = MakeAffineMatrix(cameraScale_, cameraRotate_, cameraPos_);
	viewMatrix_ = Inverse(cameraWorldMatrix_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

#endif 

Matrix4x4 Camera::viewProjectionMatrixGetter() {
	return viewProjectionMatrix_;
}

Matrix4x4 Camera::viewportMatrixGetter() {
	return viewportMatrix_;
}