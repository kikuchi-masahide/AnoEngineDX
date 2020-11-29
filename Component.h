#pragma once

class GameObject;
class Layer;
class Scene;
class Game;

/// <summary>
/// �I�u�W�F�N�g�̋@�\��\���N���X
/// �I�u�W�F�N�g�̍X�V������R���|�[�l���g�Əo�͂�����R���|�[�l���g�͗������̊��N���X����h��������
/// </summary>
class Component {
public:
	/// <param name="_order">�D��x �����قǐ�ɌĂяo�����</param>
	Component(GameObject* _owner, boost::shared_ptr<std::set<ComponentHandle*>> _hset, int _order = 0);
	/// <summary>
	/// ���g�̏I���������s��
	/// (���N���X�Ŏ������RemoveComponent(this)������)
	/// </summary>
	/// <summary>
	/// �R���|�|�l���g�̍X�V����
	/// </summary>
	/// <param name="">�o�ߎ���</param>
	virtual void Update() = 0;
	int GetUpdPriority() const { return mUpdPriority; };
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Component();
	Scene& GetScene() const;
	Game& GetGame() const;
protected:
	int mUpdPriority;
	bool mDeleteFlag;
	GameObject& mOwner;
private:
	//�������w���n���h���̏W���̃|�C���^
	boost::shared_ptr<std::set<ComponentHandle*>> mHandles;
};