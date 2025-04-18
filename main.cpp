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

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_3_01";

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

	// 座標
	Vector3 translates[3] = {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f}
	};

	// 角度
	Vector3 rotate[3] = {
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f}
	};

	// 拡縮
	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f}
	};

	// 肩、肘、手のワールド行列
	Matrix4x4 worldMatrix[3]{};
	worldMatrix[0] = MakeAffineMatrix(scales[0], rotate[0], translates[0]);
	worldMatrix[1] = Multiply(MakeAffineMatrix(scales[1], rotate[1], translates[1]), worldMatrix[0]);
	worldMatrix[2] = Multiply(MakeAffineMatrix(scales[2], rotate[2], translates[2]),worldMatrix[1]);

	// 肩、肘、手の中心点
	Vector3 ScreenPos[3]{};
	ScreenPos[0] = Transform(Transform(translates[0], camera.GetViewProjectionMatrix()), camera.GetViewportMatrix());
	ScreenPos[1] = Transform(Transform(translates[1], Multiply(worldMatrix[0], camera.GetViewProjectionMatrix())), camera.GetViewportMatrix());
	ScreenPos[2] = Transform(Transform(translates[2], Multiply(worldMatrix[1], camera.GetViewProjectionMatrix())), camera.GetViewportMatrix());

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
		ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotate[0]", &rotate[0].x, 0.01f);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotate[1]", &rotate[1].x, 0.01f);
		ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotate[2]", &rotate[2].x, 0.01f);
		ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);
		ImGui::End();
#endif 

		// 肩、肘、手のワールド行列
		worldMatrix[0] = MakeAffineMatrix(scales[0], rotate[0], translates[0]);
		worldMatrix[1] = Multiply(MakeAffineMatrix(scales[1], rotate[1], translates[1]), worldMatrix[0]);
		worldMatrix[2] = Multiply(MakeAffineMatrix(scales[2], rotate[2], translates[2]), worldMatrix[1]);

		// 肩、肘、手の中心点
		ScreenPos[0] = Transform(Transform(translates[0], camera.GetViewProjectionMatrix()), camera.GetViewportMatrix());
		ScreenPos[1] = Transform(Transform(translates[1], Multiply(worldMatrix[0], camera.GetViewProjectionMatrix())), camera.GetViewportMatrix());
		ScreenPos[2] = Transform(Transform(translates[2], Multiply(worldMatrix[1], camera.GetViewProjectionMatrix())), camera.GetViewportMatrix());

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.GetViewProjectionMatrix(), camera.GetViewportMatrix());
		
		// 肩の描画
		DrawObject3D::DrawSphere(worldMatrix[0], 0.1f, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), 0xFF0000FF);
		// 肘の描画
		DrawObject3D::DrawSphere(worldMatrix[1], 0.1f, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), 0x00FF00FF);
		// 手の描画
		DrawObject3D::DrawSphere(worldMatrix[2], 0.1f, camera.GetViewProjectionMatrix(), camera.GetViewportMatrix(), 0x0000FFFF);

		// 肩、肘、手を繋げる線を描画
		Novice::DrawLine(static_cast<int>(ScreenPos[0].x), static_cast<int>(ScreenPos[0].y),
			static_cast<int>(ScreenPos[1].x), static_cast<int>(ScreenPos[1].y), 0xFFFFFFFF);
		Novice::DrawLine(static_cast<int>(ScreenPos[1].x), static_cast<int>(ScreenPos[1].y),
			static_cast<int>(ScreenPos[2].x), static_cast<int>(ScreenPos[2].y), 0xFFFFFFFF);

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
