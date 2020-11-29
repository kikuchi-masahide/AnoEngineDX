#pragma once

#include "ComponentHandle.h"
#include "Component.h"

class Game;
class GameObject;
class GameObjectHandle;

/// <summary>
/// �V�[����\���N���X
/// </summary>
class Scene {
public:
	//�R���X�g���N�^���Ăяo����̂�_game���g��AddChild�̂�
	Scene(Game* _game);
	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update();
	virtual void UniqueUpdate();
	/// <summary>
	/// �o�͊֐�
	/// </summary>
	void Output();
	virtual void UniqueOutput();
	GameObjectHandle AddObject(Vector2 _pos, double _scale, double _angle);
	Game &mGame;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	//���݂̃V�[���̃|�C���^�����Ă�̂�Game�݂̂ł��邩��C
	//�f�X�g���N�^���Ăяo����̂�Game�̂�
	virtual ~Scene();
	//���̃V�[���ɍX�V�E�o�̓R���|�[�l���g��ǉ�����
	//GameObject::AddUpdate�EOutputComponent����Ăяo�����
	void AddOutputComponent(GameObject* _obj, ComponentHandle& _handle);
	void AddUpdateComponent(GameObject* _obj, ComponentHandle& _handle);
protected:
	bool mDeleteFlag;
private:
	//���g�̎���GameObject�̃��X�g�y�ѕۗ����̃I�u�W�F�N�g
	std::list<boost::shared_ptr<GameObject>> mObjs;
	std::vector<boost::shared_ptr<GameObject>> mPandingObjs;
	//�R���|�[�l���g������set�̂��߂̏�����r�t�@���N�^
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle& left, const ComponentHandle& right) const {
			return left->mUpdPriority < right->mUpdPriority;
		}
	};
	//���g�̎��X�V�E�o�̓R���|�[�l���g�̃��X�g�C����ѕۗ��R���|�[�l���g
	std::set<ComponentHandle, ComponentHandleCompare> mUpdateComponents;
	std::vector<ComponentHandle> mPandingUpdateComponents;
	std::set<ComponentHandle, ComponentHandleCompare> mOutputComponents;
	std::vector<ComponentHandle> mPandingOutputComponents;
	//�����̎��S�X�V�E�o�̓R���|�[�l���g��Update���Ăяo��(�ۗ��R���|�[�l���g�̂���͎��s���Ȃ�)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	//�R���|�[�l���g�E�I�u�W�F�N�g�𒼐ڃ��X�g�ɓ�����邩?
	bool mIsObjCompAddable;
	//Delete�t���O�������Ă���R���|�[�l���g�E�I�u�W�F�N�g��ۗ����̂����̏���
	void DeleteAndProcessPandingObjComp();
};