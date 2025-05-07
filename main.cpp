#include <Novice.h>
#include<iostream>
#include"Math.h"
#include"DrawScreenPrintf.h"
#include"Camera.h"
#include"DrawObject3d.h"
#include<imgui.h>

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

const char kWindowTitle[] = "LE2A_05_サイトウ_アオイ_MT3_2_00";

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

	// 点の座標
	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	// 正射影ベクトル
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	// 最近接点
	Vector3 closestPoint = ClosestPoint(point, segment);

	// 点を描画するための球の変数
	Sphere pointSphere{ point,0.01f };
	Sphere closestPointSphere{ closestPoint,0.01f };
	 
	// 線を描画するための変数
	Vector3 start = Transform(Transform(segment.origin, camera.viewProjectionMatrixGetter()), camera.viewportMatrixGetter());
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), camera.viewProjectionMatrixGetter()), camera.viewportMatrixGetter());

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

		ImGui::Begin("DebugWindow");
		ImGui::DragFloat3("point", &point.x, 0.01f);
		ImGui::DragFloat3("segment_origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment_diff", &segment.diff.x, 0.01f);
		ImGui::InputFloat3("project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("closestPoint", &closestPoint.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();
#endif 

		// 正射影ベクトル
		project = Project(Subtract(point, segment.origin), segment.diff);
		// 最近接点
		closestPoint = ClosestPoint(point, segment);
		// 点の更新処理
		pointSphere = { point,0.01f };
		closestPointSphere = { closestPoint,0.01f };

		// Lineの更新処理
		start = Transform(Transform(segment.origin, camera.viewProjectionMatrixGetter()), camera.viewportMatrixGetter());
		end = Transform(Transform(Add(segment.origin, segment.diff), camera.viewProjectionMatrixGetter()), camera.viewportMatrixGetter());

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawObject3D::DrawGrid(camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter());

		// 線を描画
		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), 0xFFFFFFFF);

		// 点を描画
		DrawObject3D::DrawSphere(pointSphere, camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0xFF0000FF);
		DrawObject3D::DrawSphere(closestPointSphere, camera.viewProjectionMatrixGetter(), camera.viewportMatrixGetter(), 0x000000FF);
			
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
