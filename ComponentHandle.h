#pragma once

class Component;

class ComponentHandle {
public:
	//コピーコンストラクタ
	ComponentHandle(const ComponentHandle& _handle);
	//GameObjectのAddChildでのみ使われるコンストラクタ
	ComponentHandle(Component* _obj, boost::shared_ptr<std::set<ComponentHandle*>> _set);
	ComponentHandle();
	~ComponentHandle();
	Component* operator->() const noexcept;
	bool IsValid() const;
	void Reset(Component* _comp);
private:
	//ハンドルが指すコンポーネント
	Component* mComp;
	//mCompを指すハンドルのsetのポインタ
	boost::shared_ptr<std::set<ComponentHandle*>> mHandleSet;
};