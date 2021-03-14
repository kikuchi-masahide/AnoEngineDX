#include "InputSystem.h"
#include "Game.h"
#include "window.h"

InputSystem::InputSystem(Game* _game)
	:mGame(_game)
{
	BOOST_ASSERT_MSG(mGame != nullptr, "InputSystem::InputSystem() should be called only in Game::Game()");
	mCurIndex = 0;
	GetKeyboardState(mKeyState[mCurIndex]);
	POINT point;
	GetCursorPos(&point);
	mMousePos[mCurIndex] = GetVector2(point.x, point.y);
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
	mMousePos[mCurIndex] = GetVector2(point.x, point.y);
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

Vector2 InputSystem::GetMouseMove()
{
	auto vec = mMousePos[mCurIndex] - mMousePos[1 - mCurIndex];
	//スクリーン座標y座標は下向きなので反転
	boost::qvm::Y(vec) *= -1;
	return vec;
}

Vector2 InputSystem::GetMouseScreenPos()
{
	return mMousePos[mCurIndex];
}

Vector2 InputSystem::GetMouseClientPos(unsigned int _windowid)
{
	//スクリーン座標
	POINT mousepos;
	mousepos.x = boost::qvm::X(mMousePos[mCurIndex]);
	mousepos.y = boost::qvm::Y(mMousePos[mCurIndex]);
	auto window = mGame->GetWindow(_windowid);
	auto hwnd = window->GetWindowHandle();
	//クライアント座標に変換
	ScreenToClient(hwnd, &mousepos);
	//ウィンドウの高さを入手
	auto windowheight = window->GetWindowSize().bottom - window->GetWindowSize().top;
	//左下原点に変換
	mousepos.y = windowheight - mousepos.y;
	return GetVector2(mousepos.x, mousepos.y);
}
