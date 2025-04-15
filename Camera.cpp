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
	//cameraWorldMatrix_ = MakeAffineMatrix(scale, rotate, cameraPos);(デバックではない時のカメラのMatrix)
	// デバック時のカメラのMatrix
	transMatrix_ = MakeTranslateMatrix(cameraPos_);
	rotateMatrix_ = Multiply(MakeRotateXMatrix(cameraRotate_.x), Multiply(MakeRotateYMatrix(cameraRotate_.y), MakeRotateZMatrix(cameraRotate_.z)));
	scaleMatrix_ = MakeScaleMatrix(cameraScale_);
	cameraWorldMatrix_ = Multiply(transMatrix_, Multiply(scaleMatrix_, rotateMatrix_));
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

void Camera::DrawCameraDebugWindow(InPut& input) {
	// カメラのデバック
	ImGui::Begin("DebugCameraWindow");
	ImGui::DragFloat3("cameraPos", &cameraPos_.x, 0.02f);
	ImGui::DragFloat3("cameraRotate", &cameraRotate_.x, 0.02f);
	ImGui::DragFloat3("cameraScale", &cameraScale_.x, 0.02f);
	// カメラをリセット
	if (ImGui::Button("Reset")) {
		// カメラの位置をリセット
		cameraPos_ = { 0.0f, 0.0f, -6.49f };
		cameraRotate_ = { 0.26f,0.0f,0.0f };
		cameraScale_ = { 1.0f,1.0f,1.0f };
		// SRTMatrixの更新処理
		transMatrix_ = MakeTranslateMatrix(cameraPos_);
		rotateMatrix_ = Multiply(MakeRotateXMatrix(cameraRotate_.x), Multiply(MakeRotateYMatrix(cameraRotate_.y), MakeRotateZMatrix(cameraRotate_.z)));
		scaleMatrix_ = MakeScaleMatrix(cameraScale_);
		// カメラの変更した内容を適用する処理
		cameraWorldMatrix_ = Multiply(transMatrix_, Multiply(scaleMatrix_, rotateMatrix_));
		viewMatrix_ = Inverse(cameraWorldMatrix_);
		viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
	}
	ImGui::End();

	// ImGuiが操作されていないときにカメラの操作可能
	if (!ImGui::IsAnyItemActive()) {
		// マウスの真ん中が押された時、カメラを平行移動
		if (input.mouse.middle) {
			// X軸に移動
			if (input.mouse.mousePos.x != input.preMouse.mousePos.x) {
				// 右にずらせば右に、左にずらせば左に移動する
				if (input.mouse.mousePos.x >= input.preMouse.mousePos.x) {
					cameraPos_.x += 0.02f;
				} else {
					cameraPos_.x -= 0.02f;
				}
			}
			// Y軸に移動
			if (input.mouse.mousePos.y != input.preMouse.mousePos.y) {
				// 上にずらせば上に、下にずらせば下に移動する
				if (input.mouse.mousePos.y <= input.preMouse.mousePos.y) {
					cameraPos_.y += 0.02f;
				} else {
					cameraPos_.y -= 0.02f;
				}
			}
		}

		// 左クリックが押された時、カメラを回転
		if (input.mouse.left) {
			// Y軸に回転させる
			if (input.mouse.mousePos.x != input.preMouse.mousePos.x) {
				// 右にずらせば物体が反時計回りに回転。左にずらせば物体が時計回りに回転
				if (input.mouse.mousePos.x >= input.preMouse.mousePos.x) {
					cameraRotate_.y += 0.05f;
				} else {
					cameraRotate_.y -= 0.05f;
				}
			}
			// X軸を回転させる
			if (input.mouse.mousePos.y != input.preMouse.mousePos.y) {
				// 上にずらせば物体の下側を見れる。下にずらせば物体の上側を見れる
				if (input.mouse.mousePos.y >= input.preMouse.mousePos.y) {
					cameraRotate_.x += 0.05f;
				} else {
					cameraRotate_.x -= 0.05f;
				}
			}
		}

		// ホイールを回した時、カメラの拡縮
		if (input.mouse.wheel != input.preMouse.wheel) {
			cameraScale_.x += static_cast<float>(input.mouse.wheel) * 0.001f;
			cameraScale_.y += static_cast<float>(input.mouse.wheel) * 0.001f;
			cameraScale_.z += static_cast<float>(input.mouse.wheel) * 0.001f;
		}

		// カメラの操作おこなった場合、カメラのMatrixを変換する
		if (input.mouse.middle || input.mouse.left || (input.mouse.wheel != input.preMouse.wheel)) {
			// SRTMatrixの更新処理
			transMatrix_ = MakeTranslateMatrix(cameraPos_);
			rotateMatrix_ = Multiply(MakeRotateXMatrix(cameraRotate_.x), Multiply(MakeRotateYMatrix(cameraRotate_.y), MakeRotateZMatrix(cameraRotate_.z)));
			scaleMatrix_ = MakeScaleMatrix(cameraScale_);
			// カメラの変更した内容を適用する処理
			cameraWorldMatrix_ = Multiply(transMatrix_, Multiply(scaleMatrix_, rotateMatrix_));
			viewMatrix_ = Inverse(cameraWorldMatrix_);
			viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
		}
	}
}

#endif 

Matrix4x4 Camera::viewProjectionMatrixGetter() {
	return viewProjectionMatrix_;
}

Matrix4x4 Camera::viewportMatrixGetter() {
	return viewportMatrix_;
}