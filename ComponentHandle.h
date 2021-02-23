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
		BOOST_ASSERT_MSG(mComp == _comp, "ComponentHandle::Reset() should be called in Component::~Component()");
		mComp = nullptr;
		mHandleSet.reset();
	}
	bool operator!() const noexcept
	{
		return !static_cast<bool>(*this);
	}
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
	//bool型でキャストを完全特殊化 ダングリングハンドルであるか否か
	explicit operator bool() const noexcept
	{
		return (mComp != nullptr);
	}
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