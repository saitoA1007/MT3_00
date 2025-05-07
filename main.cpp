#include <Novice.h>
#include<iostream>
#include"Math.h"
#include"DrawScreenPrintf.h"
#include"Camera.h"
#include"DrawObject3d.h"
#include<imgui.h>
#include"Collision.h"
#include"InPutProcess.h"
#include"Physics3d.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_4_00";

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

	// バネ
	Spring spring = {};
	spring.anchor = { 0.0f,0.0f,0.0f };
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;
	// スクリーン座標のバネのアンカー位置
	Vector3 screenAnchorPos = Transform(Transform(spring.anchor, camera.GetViewProjectionMatrix()), camera.GetViewportMatrix());

	// ボール
	Ball ball = {};
	ball.pos = { 1.2f,0.0f,0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = 0x0000FFFF;
	// スクリーン座標のボール位置
	Vector3 screenBallPos = Transform(Transform(ball.pos, camera.GetViewProjectionMatrix()), camera.GetViewportMatrix());

	// デルタタイム
	float deltaTime = 1.0f / 60.0f;

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
		if (ImGui::Button("start")) {
			ball.pos = { 1.2f,0.0f,0.0f };
		}
		ImGui::End();
#endif 

		// バネの動きの処理
		simulateSpringMovement(spring, &ball, deltaTime);

		// スクリーン座標のバネのアンカー位置の更新処理
		screenAnchorPos = Transform(Transform(spring.anchor, camera.GetViewProjectionMatrix()), camera.GetViewportMatrix());
		// スクリーン座標のボール位置の更新処理
		screenBallPos = Transform(Transform(ball.pos, camera.GetViewProjectionMatrix()), camera.GetViewportMatrix());

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.GetViewProjectionMatrix(), camera.GetViewportMatrix());

		// バネのアンカーとボールを繋ぐ線の描画
		Novice::DrawLine(static_cast<int>(screenAnchorPos.x), static_cast<int>(screenAnchorPos.y),
			static_cast<int>(screenBallPos.x), static_cast<int>(screenBallPos.y), 0xFFFFFFFF);

		// ボールの描画
		DrawObject3D::DrawSphere({ball.pos,ball.radius}, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), ball.color);

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
