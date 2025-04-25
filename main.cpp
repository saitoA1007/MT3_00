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

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_4_03";

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

	// 球
	Sphere sphere = { {0.0f,0.2f,0.0f},0.05f };
	// スクリーン座標に変換
	Vector3 screenSphere = camera.TransScreen(sphere.center);

	// 円錐振り子
	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f,1.0f,0.0f };
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;
	// 円錐振り子の原点
	Vector3 screenOrigin = camera.TransScreen(conicalPendulum.anchor);

	// デルタタイム
	float deltaTime = 1.0f / 60.0f;

	// 円錐振り子運動を実行する処理
	bool isConicalPendulumStart = false;

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
			if (isConicalPendulumStart) {
				isConicalPendulumStart = false;
			} else {
				isConicalPendulumStart = true;
			}
		}
		ImGui::DragFloat("Length", &conicalPendulum.length,0.01f);
		ImGui::DragFloat("halfApexAngle", &conicalPendulum.halfApexAngle,0.01f);
		ImGui::End();
#endif 

		// フラグがtrueの時、円錐振り子運動がスタートする
		if (isConicalPendulumStart) {
		
			// 円錐振り子の処理
			ConicalPendulumMotion(conicalPendulum, sphere.center, deltaTime);

			// 円錐振り子の原点
			screenOrigin = camera.TransScreen(conicalPendulum.anchor);
			// 球の中心座標をスクリーン座標に変換
			screenSphere = camera.TransScreen(sphere.center);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.GetViewProjectionMatrix(), camera.GetViewportMatrix());

		// 円錐振り子の線を描画
		Novice::DrawLine(static_cast<int>(screenOrigin.x), static_cast<int>(screenOrigin.y),
			static_cast<int>(screenSphere.x), static_cast<int>(screenSphere.y), 0xFFFFFFFF);

		// 球の描画
		DrawObject3D::DrawSphere(sphere, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), 0xFFFFFFFF);

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
