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

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_04_04";

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
	
	// 平面
	Plane plane;
	plane.normal = Normalize({ -0.2f,0.9f,-0.3f });
	plane.distance = 0.0f;

	// ボール
	Ball ball{};
	ball.pos = { 0.8f,1.2f,0.3f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = 0xFFFFFFFF;
	ball.acceleration = { 0.0f,-9.8f,0.0f };
	// 反発係数
	Vector3 e = {0.0f,0.8f,0.0f};

	// デルタタイム
	float deltaTime = 1.0f / 60.0f;

	// 円錐振り子運動を実行する処理
	bool isStart = false;

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
			if (isStart) {
				isStart = false;
			} else {
				isStart = true;
			}
		}
		ImGui::DragFloat3("ball.pos", &ball.pos.x, 0.01f);
		if (ImGui::Button("Reset")) {
			ball.pos = { 0.8f,1.2f,0.3f };
			ball.velocity = { 0.0f,0.0f,0.0f };
		}
		ImGui::End();
#endif 

		// フラグがtrueの時、円錐振り子運動がスタートする
		if (isStart) {

			ball.velocity += ball.acceleration * deltaTime;
			ball.pos += ball.velocity * deltaTime;
			if (IsSpherePlaneCollision({ ball.pos,ball.radius }, plane)) {
				Vector3 reflected = Reflect(ball.velocity, plane.normal);
				Vector3 projectToNormal = Project(reflected, plane.normal);
				Vector3 movingDirection = reflected - projectToNormal;
				ball.velocity = projectToNormal * e + movingDirection;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.GetViewProjectionMatrix(), camera.GetViewportMatrix());

		// 平面を描画
		DrawObject3D::DrawPlane(plane, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), 0xFFFFFFFF);

		// 球の描画
		DrawObject3D::DrawSphere({ball.pos,ball.radius}, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), 0xFFFFFFFF);

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
