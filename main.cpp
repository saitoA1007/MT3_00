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

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_2_01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	//======================================================
	// 宣言と初期化
	//======================================================

	// キー入力結果を受け取る箱
	InPut input;

	// カメラのクラス
	Camera camera = Camera({ 1.0f,1.0f,1.0f }, {0.26f,0.0f,0.0f}, { 0.0f, 0.0f, -6.49f }, kWindowWidth, kWindowHeight);

	// 球の描画
	Sphere sphere[2] = {
		{{0.0f,0.0f,0.0f},0.5f},
		{1.0f,0.0f,1.0f,0.2f}
	};

	// 当たり判定のフラグ
	bool isSphereHit = false;

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
		ImGui::DragFloat3("sphere[0].center", &sphere[0].center.x, 0.01f);
		ImGui::DragFloat("sphere[0].radius", &sphere[0].radius, 0.01f);
		ImGui::DragFloat3("sphere[1].center", &sphere[1].center.x, 0.01f);
		ImGui::DragFloat("sphere[1].radius", &sphere[1].radius, 0.01f);
		ImGui::End();

#endif 

		// 球同士の衝突判定の処理
		isSphereHit = IsCollision(sphere[0], sphere[1]);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter());

		// 点を描画
		if (isSphereHit) {
			DrawObject3D::DrawSphere(sphere[0], camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFF0000FF);
		} else {
			DrawObject3D::DrawSphere(sphere[0], camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFFFFFFFF);
		}
		DrawObject3D::DrawSphere(sphere[1], camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFFFFFFFF);
			
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
