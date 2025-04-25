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

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_4_01";

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
	Sphere sphere = { {1.0f,0.0f,0.0f},0.05f };

	// 回転する時の原点
	Vector3 origin = { 0.0f,0.0f,0.0f };
	float radius = std::fabsf(Length(origin - sphere.center));
	float angularVelocity = 3.14f;
	float angle = 0.0f;

	// 等速円運動の速度
	Vector3 velocity = { 0.0f,0.0f,0.0f };

	// デルタタイム
	float deltaTime = 1.0f / 60.0f;

	// 円運動を実行する処理
	bool isRotateStart = false;

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
			isRotateStart = true;
		}
		ImGui::End();
#endif 

		// フラグがtrueの時、円運動がスタートする
		if (isRotateStart) {
			// 角度を増やしていく
			angle += angularVelocity * deltaTime;
			// 一周したら角度を0に戻す
			if (angularVelocity * 2.0f <= angle) {
				angle = 0.0f;
			}

			// 球の座標を更新する
			sphere.center.x = origin.x + std::cosf(angle) * radius;
			sphere.center.y = origin.y + std::sinf(angle) * radius;
			sphere.center.z = origin.z;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.GetViewProjectionMatrix(), camera.GetViewportMatrix());

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
