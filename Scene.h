#pragma once

class Game;
class Layer;
class GameObject;
class GameObjectHandle;
class ComponentHandle;
//�R���|�[�l���g���V�[���ŗD��x���ɕێ����邽�߂̃t�@���N�^
class ComponentHandleCompare;

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
	void CommonUpdate();
	virtual void UniqueUpdate();
	/// <summary>
	/// �o�͊֐�
	/// </summary>
	void Output();
	void CommonOutput();
	virtual void UniqueOutput();
	/// <summary>
	/// ���C���[��������
	/// </summary>
	void AddLayer(boost::shared_ptr<Layer> _layer);
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
	/// <summary>
	/// �S���C���[��Update()���Ăяo��
	/// </summary>
	void UpdateLayers();
	/// <summary>
	/// �S���C���[��Output()���Ăяo��
	/// </summary>
	void OutputLayers();
	std::vector<boost::shared_ptr<Layer>> mLayers;
	std::vector<boost::shared_ptr<Layer>> mPendingLayers;
	void InsertLayer(boost::shared_ptr<Layer> _layer);
	void DeleteLayers();
	bool mDeleteFlag;
private:
	//���g�̎���GameObject�̃��X�g�y�ѕۗ����̃I�u�W�F�N�g
	std::list<boost::shared_ptr<GameObject>> mObjs;
	std::vector<boost::shared_ptr<GameObject>> mPandingObjs;
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