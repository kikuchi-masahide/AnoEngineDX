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
	Component(boost::weak_ptr<GameObject>& _owner, int _order = 0);
	/// <summary>
	/// ���g�̏I���������s��
	/// (���N���X�Ŏ������RemoveComponent(this)������)
	/// </summary>
	/// <summary>
	/// �R���|�|�l���g�̍X�V����
	/// </summary>
	/// <param name="">�o�ߎ���</param>
	virtual void Update();
	int GetUpdPriority() const { return mUpdPriority; };
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Component();
	boost::weak_ptr<Layer> GetLayer() const;
	boost::weak_ptr<Scene> GetScene() const;
	boost::weak_ptr<Game> GetGame() const;
	void SetWeakThis(boost::shared_ptr<Component>& _this);
	boost::weak_ptr<Component> GetWeakThis() const;
	const boost::weak_ptr<GameObject> mOwner;
protected:
	boost::weak_ptr<Component> mThis;
	int mUpdPriority;
	bool mDeleteFlag;
};