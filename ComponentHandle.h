#pragma once

class Component;

class ComponentHandle {
public:
	//�R�s�[�R���X�g���N�^
	ComponentHandle(const ComponentHandle& _handle);
	//GameObject��AddChild�ł̂ݎg����R���X�g���N�^
	ComponentHandle(Component* _obj, boost::shared_ptr<std::set<ComponentHandle*>> _set);
	ComponentHandle();
	~ComponentHandle();
	Component* operator->() const noexcept;
	bool IsValid() const;
	void Reset(Component* _comp);
private:
	//�n���h�����w���R���|�[�l���g
	Component* mComp;
	//mComp���w���n���h����set�̃|�C���^
	boost::shared_ptr<std::set<ComponentHandle*>> mHandleSet;
};