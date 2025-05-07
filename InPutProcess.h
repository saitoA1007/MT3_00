#pragma once
#include"Vector2.h"

// キー入力結果を受け取る箱
class InPut {
public:

	// パッド変数
	struct Pad {
		bool up = false;				//十字キー上
		bool down = false;				//十字キー下
		bool left = false;				//十字キー左
		bool right = false;				//十字キー右
		bool start = false;				//三キー
		bool back = false;				//コピー(二重の四角)キー
		bool L1 = false;				//Lキー
		bool L2 = false;				//LTキー
		bool R1 = false;				//Rキー
		bool R2 = false;				//RTキー
		bool A = false;					//Aキー
		bool B = false;					//Bキー
		bool X = false;					//Xキー
		bool Y = false;					//Yキー
		bool LStickPless = false;		//Lスティック押込み
		Vector2 LStickDirection = {};	//Lスティック角度
		bool RStickPless = false;		//Rスティック押込み
		Vector2 RStickDirection = {};	//Rスティック角度
	};

	struct Mouse {
		bool left = false;     // 左クリック
		bool right = false;    // 右クリック
		bool middle = false;   // 真ん中
		Vector2 mousePos{};    // マウスの位置を取得(スクリーン座標系)
		int wheel = 0;         // ホイールスクロール量(奥に回すと+、手前に回すと-)
	};

	/// <summary>
	/// 入力を受け付ける処理
	/// </summary>
	void InPutProcess();

public:
	// キーボード
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// パッド
	Pad pad;
	Pad prePad;

	// マウス
	Mouse mouse;
	Mouse preMouse;
};

