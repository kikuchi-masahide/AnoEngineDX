#include "InputSystem.h"

InputSystem::InputSystem(Game* _game)
	:mGame(_game)
{
	BOOST_ASSERT_MSG(mGame != nullptr, "InputSystem::InputSystem() should be called only in Game::Game()");
	mCurIndex = 0;
	GetKeyboardState(mKeyState[mCurIndex]);
	POINT point;
	GetCursorPos(&point);
	mMousePos[mCurIndex] = MatVec::Vector2(point.x, point.y);
}

void InputSystem::ProcessBeforeUpdate(Game* _game)
{
	BOOST_ASSERT_MSG(mGame == _game, "only Game::ProcessInput() can call InputSystem::ProcessBeforeUpdate()");
	//mCurIndexの反転
	mCurIndex = 1 - mCurIndex;
	//現在の状態をmKeyState[mCurIndex]に保存
	GetKeyboardState(mKeyState[mCurIndex]);
	//カーソル位置の取得
	POINT point;
	GetCursorPos(&point);
	mMousePos[mCurIndex] = MatVec::Vector2(point.x, point.y);
}

ButtonState InputSystem::GetKeyState(unsigned char _key)
{
	//前フレームで入力がされていた
	if (mKeyState[1 - mCurIndex][_key] & 0x80)
	{
		if (mKeyState[mCurIndex][_key] & 0x80)
		{
			return ButtonState::Held;
		}
		else
		{
			return ButtonState::Released;
		}
	}
	else
	{
		if (mKeyState[mCurIndex][_key] & 0x80)
		{
			return ButtonState::Pressed;
		}
		else
		{
			return ButtonState::None;
		}
	}
}

MatVec::Vector2 InputSystem::GetMouseMove()
{
	MatVec::Vector2 vec = mMousePos[mCurIndex] - mMousePos[1 - mCurIndex];
	//スクリーン座標y座標は下向きなので反転
	vec(1) *= -1;
	return vec;
}

MatVec::Vector2 InputSystem::GetMouseScreenPos()
{
	return mMousePos[mCurIndex];
}