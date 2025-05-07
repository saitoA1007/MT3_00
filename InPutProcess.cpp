#include"InPutProcess.h"
#include<Novice.h>

void InPut::InPutProcess() {

	// 仮入力用変数
	int x, y;

	//======================================================
	// キーボード入力の処理
	//======================================================

	// キーボード入力を同期させる
	memcpy(preKeys, keys, 256);
	// キーボード入力
	Novice::GetHitKeyStateAll(keys);

	//========================================================
	// パッド入力の処理
	//========================================================

	//パッド入力の前の操作を格納する
	prePad = pad;
	//パッドボタン入力
	pad.up = Novice::IsPressButton(0, PadButton::kPadButton0);
	pad.down = Novice::IsPressButton(0, PadButton::kPadButton1);
	pad.left = Novice::IsPressButton(0, PadButton::kPadButton2);
	pad.right = Novice::IsPressButton(0, PadButton::kPadButton3);
	pad.start = Novice::IsPressButton(0, PadButton::kPadButton4);
	pad.back = Novice::IsPressButton(0, PadButton::kPadButton5);
	pad.LStickPless = Novice::IsPressButton(0, PadButton::kPadButton6);
	pad.RStickPless = Novice::IsPressButton(0, PadButton::kPadButton7);
	pad.L1 = Novice::IsPressButton(0, PadButton::kPadButton8);
	pad.R1 = Novice::IsPressButton(0, PadButton::kPadButton9);
	pad.L2 = Novice::IsPressButton(0, PadButton::kPadButton14);		//そのままだとAと同じだからNovice.cppのIsPressXInputButtonのトリガーL2/R2特殊処理を10から14に変更する
	pad.R2 = Novice::IsPressButton(0, PadButton::kPadButton15);		//そのままだとBと同じだからNovice.cppのIsPressXInputButtonのトリガーL2/R2特殊処理を11から15に変更する
	pad.A = Novice::IsPressButton(0, PadButton::kPadButton10);
	pad.B = Novice::IsPressButton(0, PadButton::kPadButton11);
	pad.X = Novice::IsPressButton(0, PadButton::kPadButton12);
	pad.Y = Novice::IsPressButton(0, PadButton::kPadButton13);
	//パッドスティック角度入力
	Novice::GetAnalogInputLeft(0, &x, &y);
	pad.LStickDirection = { static_cast<float>(x),static_cast<float>(y) };
	Novice::GetAnalogInputRight(0, &x, &y);
	pad.RStickDirection = { static_cast<float>(x),static_cast<float>(y) };

	//====================================================================
	// マウス入力処理
	//====================================================================

	// マウス入力の前の操作を格納する
	preMouse = mouse;
	// マウス入力
	mouse.left = Novice::IsPressMouse(0);
	mouse.right = Novice::IsPressMouse(1);
	mouse.middle = Novice::IsPressMouse(2);
	Novice::GetMousePosition(&x, &y);
	mouse.mousePos.x = static_cast<float>(x);
	mouse.mousePos.y = static_cast<float>(y);
	mouse.wheel = Novice::GetWheel();
}