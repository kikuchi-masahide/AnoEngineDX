#pragma once

#include "Component.h"

//Component���p�������N���X�̃|�C���^����Ɏg���n���h��
template<class T>
class ComponentHandle {
public:
	//�R�s�[�R���X�g���N�^
	ComponentHandle(const ComponentHandle<T>& _handle)
		:mComp(_handle.mComp), mHandleSet(_handle.mHandleSet)
	{
		mHandleSet->insert((void*)this);
	}
	//GameObject��AddChild�ł̂ݎg����R���X�g���N�^
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
		//�_���O�����O�|�C���^�Ȃ��assert���o��
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
	//bool�^�ւ̃L���X�g �_���O�����O�n���h���ł��邩�ۂ�
	explicit operator bool() const noexcept
	{
		return (mComp != nullptr);
	}
	bool operator!() const noexcept
	{
		return !static_cast<bool>(*this);
	}
	////Coponent���w��Handle�ւ̃L���X�g
	//explicit operator ComponentHandle<Component>() const noexcept
	//{
	//	Log::OutputTrivial("cast");
	//	return ComponentHandle<Component>(mComp, mHandleSet);
	//};
	template<class U>
	explicit operator ComponentHandle<U>() const noexcept {
		//�p���֌W�̃`�F�b�N
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
	//	//�����̉���
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
	//�n���h�����w���R���|�[�l���g
	T* mComp;
	//mComp���w���n���h����set�̃|�C���^(void*���g���̂͂ł���΂�߂���)
	boost::shared_ptr<std::set<void*>> mHandleSet;
};