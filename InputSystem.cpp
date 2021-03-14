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
	//mCurIndex�̔��]
	mCurIndex = 1 - mCurIndex;
	//���݂̏�Ԃ�mKeyState[mCurIndex]�ɕۑ�
	GetKeyboardState(mKeyState[mCurIndex]);
	//�J�[�\���ʒu�̎擾
	POINT point;
	GetCursorPos(&point);
	mMousePos[mCurIndex] = GetVector2(point.x, point.y);
}

ButtonState InputSystem::GetKeyState(unsigned char _key)
{
	//�O�t���[���œ��͂�����Ă���
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
	//�X�N���[�����Wy���W�͉������Ȃ̂Ŕ��]
	boost::qvm::Y(vec) *= -1;
	return vec;
}

Vector2 InputSystem::GetMouseScreenPos()
{
	return mMousePos[mCurIndex];
}

Vector2 InputSystem::GetMouseClientPos(unsigned int _windowid)
{
	//�X�N���[�����W
	POINT mousepos;
	mousepos.x = boost::qvm::X(mMousePos[mCurIndex]);
	mousepos.y = boost::qvm::Y(mMousePos[mCurIndex]);
	auto window = mGame->GetWindow(_windowid);
	auto hwnd = window->GetWindowHandle();
	//�N���C�A���g���W�ɕϊ�
	ScreenToClient(hwnd, &mousepos);
	//�E�B���h�E�̍��������
	auto windowheight = window->GetWindowSize().bottom - window->GetWindowSize().top;
	//�������_�ɕϊ�
	mousepos.y = windowheight - mousepos.y;
	return GetVector2(mousepos.x, mousepos.y);
}
