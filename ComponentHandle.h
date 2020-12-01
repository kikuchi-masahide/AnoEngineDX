#pragma once

#include "Component.h"

//Componentを継承したクラスのポインタ代わりに使うハンドラ
template<class T>
class ComponentHandle {
public:
	//コピーコンストラクタ
	ComponentHandle(const ComponentHandle<T>& _handle)
		:mComp(_handle.mComp), mHandleSet(_handle.mHandleSet)
	{
		mHandleSet->insert((void*)this);
	}
	//GameObjectのAddChildでのみ使われるコンストラクタ
	ComponentHandle(T* _obj, boost::shared_ptr<std::set<void*>> _set)
		:mComp(_obj), mHandleSet(_set)
	{
		mHandleSet->insert((void*)this);
	}
	ComponentHandle()
		: mComp(nullptr), mHandleSet(nullptr)
	{}
	~ComponentHandle()
	{
		if (mHandleSet != nullptr)mHandleSet->erase((void*)this);
	}
	T* operator->() const noexcept
	{
		//ダングリングポインタならばassertを出す
		if (mComp == nullptr) {
			Log::OutputCritical("Dungling Pointer");
		}
		return mComp;
	}
	bool IsValid() const
	{
		return (mComp != nullptr);
	}
	void Reset(Component* const _comp)
	{
		assert(mComp == _comp);
		mComp = nullptr;
		mHandleSet.reset();
	}
	//bool型へのキャスト ダングリングハンドルであるか否か
	explicit operator bool() const noexcept
	{
		return (mComp != nullptr);
	}
	bool operator!() const noexcept
	{
		return !static_cast<bool>(*this);
	}
	//Coponentを指すHandleへのキャスト
	explicit operator ComponentHandle<Component>() const noexcept
	{
		Log::OutputTrivial("cast");
		return ComponentHandle<Component>(mComp, mHandleSet);
	};
private:
	//ハンドルが指すコンポーネント
	T* mComp;
	//mCompを指すハンドルのsetのポインタ(void*を使うのはできればやめたい)
	boost::shared_ptr<std::set<void*>> mHandleSet;
};