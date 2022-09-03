//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "InputSystem.h"

InputSystem::InputSystem()
{
	cur_index_ = 0;
	GetKeyboardState(key_state_[cur_index_]);
	POINT point;
	GetCursorPos(&point);
	mouse_pos_[cur_index_] = MatVec::Vector2(point.x, point.y);
}

void InputSystem::ProcessBeforeUpdate()
{
	//mCurIndexの反転
	cur_index_ = 1 - cur_index_;
	//現在の状態をmKeyState[cur_index_]に保存
	GetKeyboardState(key_state_[cur_index_]);
	//カーソル位置の取得
	POINT point;
	GetCursorPos(&point);
	mouse_pos_[cur_index_] = MatVec::Vector2(point.x, point.y);
}

ButtonState InputSystem::GetKeyState(int key) const
{
	//前フレームで入力がされていた
	if (key_state_[1 - cur_index_][key] & 0x80) {
		if (key_state_[cur_index_][key] & 0x80) {
			return ButtonState::Held;
		}
		else {
			return ButtonState::Released;
		}
	}
	else {
		if (key_state_[cur_index_][key] & 0x80) {
			return ButtonState::Pressed;
		}
		else {
			return ButtonState::None;
		}
	}
}

MatVec::Vector2 InputSystem::GetMouseMove() const
{
	MatVec::Vector2 vec = mouse_pos_[cur_index_] - mouse_pos_[1 - cur_index_];
	//スクリーン座標y座標は下向きなので反転
	vec(1) *= -1;
	return vec;
}

MatVec::Vector2 InputSystem::GetMouseScreenPos() const
{
	return mouse_pos_[cur_index_];
}