#include "UIScreen.h"

UIScreen::UIScreen(Scene* _scene, bool _inputflag, bool _updateflag)
	:mDeleteFlag(false),mDoesInputGoThrough(_inputflag),mDoesUpdateComp(_updateflag),mScene(_scene)
{
}

void UIScreen::Update()
{
}

void UIScreen::SetDeleteFlag()
{
	mDeleteFlag = true;
}

bool UIScreen::GetDeleteFlag()
{
	return mDeleteFlag;
}

bool UIScreen::DoesInputGoThrough()
{
	return mDoesInputGoThrough;
}

bool UIScreen::DoesUpdateComp()
{
	return mDoesUpdateComp;
}

void UIScreen::SetInputTransparency(bool _flag)
{
	mDoesInputGoThrough = _flag;
}

void UIScreen::SetCompUpdateFlag(bool _flag)
{
	mDoesUpdateComp = _flag;
}
