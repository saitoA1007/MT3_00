#include <Novice.h>
#include<iostream>
#include"Math.h"
#include"DrawScreenPrintf.h"
#include"Camera.h"
#include"DrawObject3d.h"
#include<imgui.h>

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_1_02";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//======================================================
	// 宣言と初期化
	//======================================================

	// カメラのクラス
	Camera camera = Camera({ 1.0f,1.0f,1.0f }, {0.26f,0.0f,0.0f}, { 0.0f, 1.9f, -6.49f }, kWindowWidth, kWindowHeight);

	// 球の変数
	Sphere sphere{ {0.0f,0.0f,0.0f},0.5f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

#ifdef _DEBUG
		// カメラのデバック
		camera.DrawCameraDebugWindow();

		// 球のデバック
		ImGui::Begin("DebugSphereWindow");
		ImGui::DragFloat3("sphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphereRadius", &sphere.radius, 0.01f);
		ImGui::End();
#endif 

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter());

		// 球の描画
		DrawObject3D::DrawSphere(sphere, camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(),0x000000FF);
			
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
