#include "ComponentHandle.h"
#include "Component.h"

ComponentHandle::ComponentHandle(const ComponentHandle& _handle)
	:mComp(_handle.mComp),mHandleSet(_handle.mHandleSet)
{
	mHandleSet->insert(this);
}

ComponentHandle::ComponentHandle(Component* _obj, boost::shared_ptr<std::set<ComponentHandle*>> _set)
	:mComp(_obj),mHandleSet(_set)
{
	mHandleSet->insert(this);
}

ComponentHandle::ComponentHandle()
	: mComp(nullptr), mHandleSet(nullptr)
{}

ComponentHandle::~ComponentHandle()
{
	if (mHandleSet != nullptr)mHandleSet->erase(this);
}

void ComponentHandle::Reset(Component* _comp)
{
	assert(mComp == _comp);
	mComp = nullptr;
	mHandleSet.reset();
}

Component* ComponentHandle::operator->() const noexcept {
	//ダングリングポインタならばassertを出す
	if (mComp == nullptr) {
		Log::OutputCritical("Dungling Pointer");
	}
	return mComp;
}

bool ComponentHandle::IsValid()
{
	return (mComp != nullptr);
}

void ComponentHandle::Reset(Component* _comp)
{
	assert(mComp == _comp);
	mComp = nullptr;
	mHandleSet.reset();
}