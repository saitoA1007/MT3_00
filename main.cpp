#include <Novice.h>
#include<iostream>
#include"Math.h"
#include"DrawScreenPrintf.h"
#include"Camera.h"
#include"DrawObject3d.h"
#include<imgui.h>
#include"Collision.h"
#include"InPutProcess.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_2_05";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	//======================================================
	// 宣言と初期化
	//======================================================

	// キー入力結果を受け取る箱
	InPut input;

	// カメラ
	Camera camera = Camera({ 1.0f,1.0f,1.0f }, { 0.26f,0.0f,0.0f }, { 0.0f, 0.0f, -6.49f }, kWindowWidth, kWindowHeight);

	// aabb1
	AABB aabb1{
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.0f,0.0f,0.0f}
	};
	// aabb2
	AABB aabb2{
		.min{0.2f,0.2f,0.2f},
		.max{1.0f,1.0f,1.0f}
	};
	
	// 当たり判定のフラグ
	bool isAABBHit = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		///
		/// ↓更新処理ここから
		///

		// 入力処理
		input.InPutProcess();

#ifdef _DEBUG
		// カメラのデバック
		camera.DrawCameraDebugWindow(input);

		ImGui::Begin("DebugWindow");
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);
		ImGui::End();

		// 最大、最小の調整処理
		aabb1.min = Min(aabb1.min, aabb1.max);
		aabb1.max = Max(aabb1.min, aabb1.max);
		aabb2.min = Min(aabb2.min, aabb2.max);
		aabb2.max = Max(aabb2.min, aabb2.max);
#endif 

		// 球と平面の衝突判定の処理
		isAABBHit = IsAABBCollision(aabb1,aabb2);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter());

		// aabb1の描画
		DrawObject3D::DrawAABB(aabb1, camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFFFFFFFF);

		// aabb2の描画
		if (isAABBHit) {
			DrawObject3D::DrawAABB(aabb2, camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFF0000FF);
		} else {
			DrawObject3D::DrawAABB(aabb2, camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFFFFFFFF);
		}
			
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (input.preKeys[DIK_ESCAPE] == 0 && input.keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
