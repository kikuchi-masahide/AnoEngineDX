//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Log.h"

class Component;

//Componentを継承したクラスのポインタ代わりに使うハンドラ
template<class T>
class ComponentHandle final {
public:
	//コピーコンストラクタ
	ComponentHandle(const ComponentHandle<T>& handle)
		:comp_(handle.comp_), handles_set_(handle.handles_set_)
	{
		if (handles_set_ != nullptr) {
			handles_set_->insert((void*)this);
		}
	}
	//GameObjectのAddChildでのみ使われるコンストラクタ
	ComponentHandle(T* obj, std::unordered_set<void*>* set)
		:comp_(obj), handles_set_(set)
	{
		if (comp_ != nullptr) {
			handles_set_->insert((void*)this);
		}
	}
	ComponentHandle()
		: comp_(nullptr), handles_set_(nullptr)
	{}
	ComponentHandle(const T* comp)
		:comp_(comp), handles_set_(comp->mHandles)
	{
		handles_set_->insert((void*)this);
	}
	ComponentHandle<T>& operator=(const ComponentHandle<T>& h) {
		Reset();
		if (h.comp_ != nullptr) {
			comp_ = h.comp_;
			handles_set_ = h.handles_set_;
			handles_set_->insert((void*)this);
		}
		return *this;
	}
	~ComponentHandle()
	{
		Reset();
	}
	T* operator->() const noexcept
	{
		//ダングリングポインタならばassertを出す
		if (comp_ == nullptr) {
			Log::OutputCritical("Dungling Pointer");
		}
		return comp_;
	}
	bool IsValid() const
	{
		return (comp_ != nullptr);
	}
	void Reset()
	{
		if (comp_ != nullptr) {
			handles_set_->erase(static_cast<void*>(this));
			handles_set_ = nullptr;
			comp_ = nullptr;
		}
	}
	template<class U>
	explicit operator ComponentHandle<U>() const noexcept {
		//継承関係のチェック
		U* self;
		try {
			self = dynamic_cast<U*>(comp_);
		}
		catch (std::bad_cast& b) {
			Log::OutputCritical("bad_cast\n");
		}
		return ComponentHandle<U>(self, handles_set_);
	}
	//bool型でキャストを完全特殊化 ダングリングハンドルであるか否か
	explicit operator bool() const noexcept
	{
		return (comp_ != nullptr);
	}
	bool operator!() const noexcept
	{
		return !static_cast<bool>(*this);
	}
	//指しているコンポーネントが同じならば等しい
	bool operator==(const ComponentHandle<T>& handle) const;
	bool operator<(const ComponentHandle<T>& handle) const
	{
		return comp_ < handle.comp_;
	}
private:
	//本当はやりたくない
	template<class U>
	friend bool operator<(const ComponentHandle<U>& handle1, const ComponentHandle<U>& handle2);
	//ハンドルが指すコンポーネント
	T* comp_;
	//mCompを指すハンドルのsetのポインタ(void*を使うのはできればやめたい)
	std::unordered_set<void*>* handles_set_;
};

template<class T>
inline bool ComponentHandle<T>::operator==(const ComponentHandle<T>& handle) const
{
	return (comp_ == handle.comp_);
}

template<class T>
//std::set等などで使う際の予備
bool operator<(const ComponentHandle<T>& handle1, const ComponentHandle<T>& handle2)
{
	return (handle1.comp_ < handle2.comp_);
}