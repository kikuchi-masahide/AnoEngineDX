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
	//template<class TYPE>
	//ComponentHandle(const ComponentHandle<TYPE>& _handle) {
	//	try {
	//		mComp = dynamic_cast<T>(_handle->mComp);
	//	}
	//	catch (std::bad_cast& b) {
	//		Log::OutputCritical("bad_cast\n");
	//	}
	//	if (mComp != nullptr)
	//	{
	//		mHandleSet = _handle->mHandleSet;
	//	}
	//}
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
	////Coponentを指すHandleへのキャスト
	//explicit operator ComponentHandle<Component>() const noexcept
	//{
	//	Log::OutputTrivial("cast");
	//	return ComponentHandle<Component>(mComp, mHandleSet);
	//};
	template<class U>
	explicit operator ComponentHandle<U>() const noexcept {
		//継承関係のチェック
		U* self;
		try {
			self = dynamic_cast<U*>(mComp);
		}
		catch (std::bad_cast& b) {
			Log::OutputCritical("bad_cast\n");
		}
		return ComponentHandle<U>(self, mHandleSet);
	}
	//template<class U>
	//ComponentHandle<T>& operator=(const ComponentHandle<U>& h)
	//{
	//	T* newcomp;
	//	try {
	//		newcomp = dynamic_cast<T*>(h.mComp);
	//	}
	//	catch (std::bad_cast& b) {
	//		Log::OutputCritical("bad_cast\n");
	//	}
	//	//自分の解除
	//	mHandleSet->erase((void*)this);
	//	mHandleSet = nullptr;
	//	mComp = nullptr;
	//	if (!newcomp)
	//	{
	//		mComp = newcomp;
	//		mHandleSet = h.mHandleSet;
	//	}
	//	return *this;
	//}
	ComponentHandle<T>& operator=(const ComponentHandle<T>& h) {
		mHandleSet->erase((void*)this);
		mHandleSet = nullptr;
		mComp = nullptr;
		if (h.mComp)
		{
			mComp = h.mComp;
			mHandleSet = h.mHandleSet;
			mHandleSet->insert((void*)this);
		}
		return *this;
	}
private:
	//ハンドルが指すコンポーネント
	T* mComp;
	//mCompを指すハンドルのsetのポインタ(void*を使うのはできればやめたい)
	boost::shared_ptr<std::set<void*>> mHandleSet;
};