//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "Math/Math.h"

/// <summary>
/// 各キー・マウスボタンの状態を表すenum
/// None:off->off
/// Pressed:off->on
/// Released:on->off
/// Held:on->on
/// </summary>
enum class ButtonState : char {
	None = 0,
	Pressed,
	Released,
	Held
};

/// <summary>
/// Gameが持つ，入力を管理するクラス
/// </summary>
class InputSystem : public boost::noncopyable {
public:
	/// <summary>
	/// Gameからのみ呼ばれうるコンストラクタ
	/// </summary>
	InputSystem();
	/// <summary>
	/// シーン，オブジェクト等のUpdateが呼び出される前の前処理(Gameのみが呼び出せる)
	void ProcessBeforeUpdate();
	/// <summary>
	/// 指定キーの状態をButtonState型で返す
	/// </summary>
	/// <param name="_key">文字キーの場合対応文字大文字のキーコード('A'など)，それ以外の場合VK_～～の該当する変数</param>
	/// <returns></returns>
	ButtonState GetKeyState(int _key) const;
	/// <summary>
	/// 前フレームから現フレームでのマウスの移動をベクトルで取得(現位置-前位置，左下原点)
	/// </summary>
	MatVec::Vector2 GetMouseMove() const;
	/// <summary>
	/// マウスのスクリーン座標を取得(左上原点はそのまま)
	/// </summary>
	MatVec::Vector2 GetMouseScreenPos() const;
private:
	//前・現フレームのキーボードの状況
	BYTE key_state_[2][256];
	//mKeyState，mMousePosでの現フレーム状況の添え字
	unsigned int cur_index_;
	//前・現フレームのマウスカーソル位置(ただしこれはスクリーン座標であることに注意)
	MatVec::Vector2 mouse_pos_[2];
};