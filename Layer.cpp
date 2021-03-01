#include "Layer.h"
#include "Scene.h"
#include "Game.h"

Layer::Layer(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap)
	:mScene(*_scene), mRect(_rect), z(_z),mHandles(_hset),mSwapchainID(_swap)
{
	BOOST_ASSERT(_scene != nullptr);
	BOOST_ASSERT(_hset != nullptr);
}

Layer::~Layer()
{
	std::for_each(mHandles->begin(), mHandles->end(), [this](void* _obj) {
		//ñ≥óùÇ‚ÇË
		((LayerHandle<Layer>*)_obj)->Reset(this);
	});
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

double Layer::GetZ()
{
	return z;
}

bool Layer::HasZChanged()
{
	return mWasZChanged;
}

Game& Layer::GetGame() const
{
	return mScene.mGame;
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
