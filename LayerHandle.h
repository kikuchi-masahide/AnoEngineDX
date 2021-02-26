#pragma once

#include "Layer.h"

template<class T>
class LayerHandle {
public:
	//コピーコンストラクタ
	LayerHandle(const LayerHandle<T>& _handle)
		:mLayer(_handle.mLayer),mHandleSet(_handle.mHandleSet)
	{
		mHandleSet->insert((void*)this);
	}
	//SceneのAddLayerから呼び出される
	LayerHandle(T* _layer, boost::shared_ptr<std::set<void*>> _set)
		:mLayer(_layer), mHandleSet(_set)
	{
		mHandleSet->insert((void*)this);
	}
	LayerHandle()
		:mLayer(nullptr), mHandleSet(nullptr)
	{}
	~LayerHandle()
	{
		if (mHandleSet != nullptr)
		{
			mHandleSet->erase((void*)this);
		}
	}
	T* operator->() const noexcept
	{
		//ダングリングポインタならばassertを出す
		BOOST_ASSERT_MSG(mLayer != nullptr, "LayerHandle dungling pointer");
		return mLayer;
	}
	bool IsValid() const
	{
		return (mLayer != nullptr);
	}
	void Reset(Layer* const _layer)
	{
		BOOST_ASSERT_MSG(mLayer == _layer, "LayerHandle::Reset() should be called in Layer::~Layer()");
		mLayer = nullptr;
		mHandleSet.reset();
	}
	bool operator!() const noexcept
	{
		return (mLayer == nullptr);
	}
	template<class U>
	explicit operator LayerHandle<U>() const noexcept {
		//継承関係のチェック
		U* self;
		try {
			self = dynamic_cast<U*>(mLayer);
		}
		catch (std::bad_cast& b) {
			Log::OutputCritical("bad_cast\n");
		}
		return LayerHandle<U>(self, mHandleSet);
	}
	//bool型でキャストを完全特殊化 ダングリングハンドルであるか否か
	explicit operator bool() const noexcept
	{
		return (mLayer != nullptr);
	}
	LayerHandle<T>& operator=(const LayerHandle<T>& h) {
		mHandleSet->erase((void*)this);
		mHandleSet = nullptr;
		mLayer = nullptr;
		if (h.mLayer)
		{
			mLayer = h.mLayer;
			mHandleSet = h.mHandleSet;
			mHandleSet->insert((void*)this);
		}
		return *this;
	}
private:
	//ハンドルが指すレイヤー
	T* mLayer;
	//mLayerを指すハンドルのset
	boost::shared_ptr<std::set<void*>> mHandleSet;
};