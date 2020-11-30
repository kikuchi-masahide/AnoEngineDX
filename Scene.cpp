#include "Scene.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "Component.h"
#include "ComponentHandle.h"

Scene::Scene(Game* _game)
	:mGame(*_game), mIsObjCompAddable(true) {
	assert(_game != nullptr);
}

void Scene::Update()
{
	//�������炵�΂炭�̊ԁC�ǉ������I�u�W�F�N�g�E�R���|�[�l���g�͕ۗ��ɓ����
	mIsObjCompAddable = false;
	UniqueUpdate();
	LaunchUpdateComponents();
}

void Scene::UniqueUpdate() {}

void Scene::Output()
{
	LaunchOutputComponents();
	UniqueOutput();
	mIsObjCompAddable = true;
	//�ۗ����Ă����I�u�W�F�N�g�E�R���|�[�l���g�̏������s��
	DeleteAndProcessPandingObjComp();
}

void Scene::UniqueOutput() {}

GameObjectHandle Scene::AddObject(Vector2 _pos, double _scale, double _angle)
{
	boost::shared_ptr<std::set<GameObjectHandle*>> objhsetp(new std::set<GameObjectHandle*>());
	boost::shared_ptr<GameObject> objp(new GameObject(this, objhsetp, _pos, _scale, _angle));
	//���ڒǉ����Ă悢�Ȃ�΂�������
	if (mIsObjCompAddable)mObjs.push_back(objp);
	else mPandingObjs.push_back(objp);
	GameObjectHandle objh(objp.get(), objhsetp);
	return objh;
}

Scene::~Scene() {}

void Scene::AddOutputComponent(GameObject* _obj, ComponentHandle& _handle)
{
	assert(_obj != nullptr);
	if (mIsObjCompAddable)mOutputComponents.insert(_handle);
	else mPandingOutputComponents.push_back(_handle);
}

void Scene::AddUpdateComponent(GameObject* _obj, ComponentHandle& _handle)
{
	assert(_obj != nullptr);
	assert(_handle.IsValid());
	if (mIsObjCompAddable)mUpdateComponents.insert(_handle);
	else mPandingUpdateComponents.push_back(_handle);
}

void Scene::LaunchUpdateComponents()
{
	for (auto itr = mUpdateComponents.begin(); itr != mUpdateComponents.end(); itr++)
	{
		(*itr)->Update();
	}
}

void Scene::LaunchOutputComponents()
{
	for (auto itr = mOutputComponents.begin(); itr != mOutputComponents.end(); itr++)
	{
		(*itr)->Update();
	}
}

void Scene::DeleteAndProcessPandingObjComp()
{
	//�ۗ����Ă����I�u�W�F�N�g�E�R���|�[�l���g��ǉ�
	for (auto& obj : mPandingObjs) {
		mObjs.push_back(obj);
	}
	mPandingObjs.clear();
	for (auto& handle : mPandingUpdateComponents) {
		mUpdateComponents.insert(handle);
	}
	mPandingUpdateComponents.clear();
	for (auto& handle : mPandingOutputComponents) {
		mOutputComponents.insert(handle);
	}
	mPandingOutputComponents.clear();
	//�S�I�u�W�F�N�g�����
	auto objitr = mObjs.begin();
	while (objitr != mObjs.end()) {
		//���̃I�u�W�F�N�g�̃t���O�������Ă���Ȃ�Ώ���
		if ((*objitr)->GetDeleteFlag())objitr = mObjs.erase(objitr);
		else {
			//�I�u�W�F�N�g�ɂ���Ȃ��R���|�[�l���g���폜������
			(*objitr)->DeleteFlagedComponents(this);
			objitr++;
		}
	}
	//�_���O�����O�n���h���폜
	auto itr = mUpdateComponents.begin();
	while (itr != mUpdateComponents.end()) {
		if (!(itr->IsValid()))itr = mUpdateComponents.erase(itr);
		else itr++;
	}
	itr = mOutputComponents.begin();
	while (itr != mOutputComponents.end()) {
		if (!(itr->IsValid()))itr = mOutputComponents.erase(itr);
		else itr++;
	}
}