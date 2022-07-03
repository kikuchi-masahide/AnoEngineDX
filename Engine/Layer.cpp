#include "Layer.h"
#include "Scene.h"
#include "Game.h"

Layer::Layer(Rect2 _rect, double _z, unsigned int _swap)
	:mRect(_rect), z(_z), mSwapchainID(_swap),mDeleteCheck(false)
{
}

Layer::~Layer()
{
	BOOST_ASSERT_MSG(mDeleteCheck == true, "irregal destructor call without Scene permission");
	while (!mHandles.empty())
	{
		auto itr = mHandles.begin();
		auto handle = static_cast<LayerHandle<Layer>*>(*itr);
		handle->Reset();
	}
}

void Layer::SetRect(Rect2 _rect)
{
	mWasRectChanged = true;
	mRectAfter = _rect;
}

Rect2 Layer::GetRect()
{
	return mRect;
}

double Layer::GetZ() const
{
	return z;
}

bool Layer::HasZChanged()
{
	return mWasZChanged;
}

void Layer::FlushZRectChange(Layer* _layer)
{
	if (mWasZChanged)
	{
		z = mZAfter;
	}
	if (mWasRectChanged)
	{
		mRect = mRectAfter;
	}
}

unsigned int Layer::GetSwapchainID()
{
	return mSwapchainID;
}

void Layer::SetZ(double _z)
{
	mWasZChanged = true;
	mZAfter = _z;
}
