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
	ComponentHandle();
	ComponentHandle(std::shared_ptr<T> comp);
	std::shared_ptr<T> operator->() const noexcept;
	void Reset() noexcept;
	bool operator!() const noexcept;
	operator bool() const noexcept;
	template<class U>
	operator ComponentHandle<U>() const noexcept;
private:
	std::weak_ptr<T> comp_;
};

template<class T>
inline ComponentHandle<T>::ComponentHandle()
{
}

template<class T>
inline ComponentHandle<T>::ComponentHandle(std::shared_ptr<T> comp)
	:comp_(comp)
{
}

template<class T>
inline std::shared_ptr<T> ComponentHandle<T>::operator->() const noexcept
{
#ifdef _DEBUG
	if (comp_.expired()) {
		Log::OutputCritical("expired ComponentHandle reference");
	}
#endif
	return comp_.lock();
}

template<class T>
inline void ComponentHandle<T>::Reset() noexcept
{
	comp_.reset();
}

template<class T>
inline bool ComponentHandle<T>::operator!() const noexcept
{
	return !static_cast<bool>(*this);
}

template<class T>
inline ComponentHandle<T>::operator bool() const noexcept
{
	return !(comp_.expired());
}

template<class T>
template<class U>
inline ComponentHandle<T>::operator ComponentHandle<U>() const noexcept
{
	//_DEBUG時のみdynamic_castを行い、無効な型変換ならばnullを返す
#ifdef _DEBUG
	if (std::shared_ptr<U> result = std::dynamic_pointer_cast<U>(comp_.lock())) {
		return ComponentHandle<U>(result);
	}
	else {
		Log::OutputCritical("bad ComponentHandle cast");
		return ComponentHandle<U>();
	}
#else
	return ComponentHandle<U>(std::static_pointer_cast<U>(comp_.lock()));
#endif
}
