//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "Component.h"
#include "ComponentHandle.h"

//Scene�ŗp����AComponent�̃R���e�i��֘A�������܂Ƃ߂��N���X
class ElementContainer
{
public:
	//�����v�[���ɓ����ɕۑ��ł���I�u�W�F�N�g�̍ő吔
	static constexpr int kMaxObjNum_ = 10000;
	//�e�����v�[���ɓ����ɕۑ��ł���Component�̍ő吔
	static constexpr int kMaxCompNum64_ = 20000;
	static constexpr int kMaxCompNum96_ = 10000;
	static constexpr int kMaxCompNum128_ = 10000;
	static void MemoryInit();
	ElementContainer();
	~ElementContainer();
	//Scene��Update�ŌĂяo���AComponent::Init���Ăяo�����߂̐�p�X���b�h�𗧂Ă�
	void CreateCompInitThread();
	//����Container�Ɋ֐����s���_�Ŋ܂܂��AUpdateComponent��Update���������s����
	void LaunchUpdateComponents();
	//����Container��OutputComponent��Update���������s����
	void LaunchOutputComponents();
	//���ݗ��܂��Ă���Comp��Init���I�������Init�X���b�h���I�������A�X���b�h�I���̃^�C�~���O�ŏ�����Ԃ�
	void FinishCompInitThread();
	//���̃t���[�����Ő������ꂽComponent��Initialize���s����ѕs�v��Component���폜����
	void ProcessPandingElements();
	//�������v�[���ɃI�u�W�F�N�g���m��
	GameObjectHandle AddObject(Scene* scene);
	//obj�̎w��GameObject��UpdateComponent��ǉ�(Scene::AddUpdateComponent����Ăяo�����)
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(std::shared_ptr<GameObject> obj, Args... args);
	//obj�̎w��GameObject��OutputComponent��ǉ�(Scene::AddOutputComponent����Ăяo�����)
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(std::shared_ptr<GameObject> obj, Args... args);
	//����GameObject���A���t���[��Output��ɍ폜����(Scene::Erase����Ăяo�����)
	void Erase(std::weak_ptr<GameObject> ptr);
	//����Component���A���t���[��Output��ɍ폜����(Scene::Erase����Ăяo�����)
	void Erase(std::weak_ptr<Component> ptr);
	//����Container�̂���GameObject/Component�����ׂĉ������
	void FreeAllElements();
	int GetGameObjectNumber() const;
	int GetUpdateComponentNumber() const;
	int GetOutputComponentNumber() const;
private:
	//�n���ꂽ�T�C�Y����AComponent���i�[���ׂ��v�[�������肷��
	static int GetSizeClass(std::size_t size);
	//GameObject��ۑ����郁�����v�[��
	//HACK:�������v�[���̃A���P�[�^��DBG_NEW�ɂł���΃��[�N�̎��ɂ��̍s�����o�͂ł��邪�A
	//���������ʏ��new�ł����[�N���̂͌��o�ł��Ă���̂ŁA�Ƃ肠�������͍l���Ă��Ȃ�
	static std::optional<boost::pool<>> obj_pool_;
	//std::shared_ptr<GameObject>�p��deleter
	static void ObjPoolDeleter(GameObject* p);
	static boost::mutex obj_pool_mutex_;
	//obj_pool_�́A�g�p�`�����N��
	static int obj_pool_used_chunk_;
	//�`�����N�T�C�Y��64,96,128�o�C�g�ł���Component�p�v�[��
	static std::optional<boost::pool<>> comp_pool_64_;
	static std::optional<boost::pool<>> comp_pool_96_;
	static std::optional<boost::pool<>> comp_pool_128_;
	//�e�������v�[������擾�����������ɑ΂���deleter(std::shared_ptr�p)
	static void CompPoolDeleter64(Component* p);
	static void CompPoolDeleter96(Component* p);
	static void CompPoolDeleter128(Component* p);
	static boost::mutex comp_pool_64_mutex_;
	static boost::mutex comp_pool_96_mutex_;
	static boost::mutex comp_pool_128_mutex_;
	//component�v�[���́A�g�p�`�����N��
	static int comp_pool_used_chunk_64_;
	static int comp_pool_used_chunk_96_;
	static int comp_pool_used_chunk_128_;
	//�������v�[����ɃA���P�[�g���R���X�g���N�^�����s���� Scene::AddUpdate/OutputComponent�p
	template<class T, class... Args>
	std::shared_ptr<T> AllocateComponentInPool(GameObjectHandle obj, Args... args);
	//Update����Component��Initialize()��ʃX���b�h�Ŏ��s����֐�
	void CompInitThreadFunc();
	//update_components_��panding_update_components_�𐮗����Z������(�}���`�X���b�h�p)
	void MergeUpdateComponents();
	//output_components_��panding_output_components_�𐮗����Z������(�}���`�X���b�h�p)
	void MergeOutputComponents();
	std::vector<std::weak_ptr<GameObject>> objs_;
	//���g�̎��X�V�E�o�̓R���|�[�l���g�̃��X�g�C����ѕۗ��R���|�[�l���g
	//HACK:�]�T��������ʂ̃R���e�i�ɕς����ꍇ�̃p�t�H�[�}���X��r
	std::vector<std::weak_ptr<Component>> update_components_;
	boost::mutex update_components_mutex_;
	std::vector<std::weak_ptr<Component>> panding_update_components_;
	std::vector<std::weak_ptr<Component>> output_components_;
	boost::mutex output_components_mutex_;
	std::vector<std::weak_ptr<Component>> panding_output_components_;
	//Update����Component��Initialize()�����s���邽�߂̃X���b�h(CreateCompInitThreadInUpdate()�ō쐬)
	boost::thread comp_init_thread_;
	//CompInitThreadFunc��condition_variable�p�̂������
	boost::condition_variable comp_init_thread_func_cond_;
	boost::mutex comp_init_thread_func_mutex_;
	//���ꂪtrue�̊Ԃ̂�comp_init_thread_in_update_�𐶑�������
	bool comp_init_thread_flag_;
	//Initiate�����s����ׂ�Component���� �ォ��ǉ����A�O������s���Ă���
	std::list<std::weak_ptr<Component>> update_comps_to_initiate_;
	boost::mutex update_comps_to_initiate_mutex_;
	std::list<std::weak_ptr<Component>> output_comps_to_initiate_;
	boost::mutex output_comps_to_initiate_mutex_;
	//�������ׂ��I�u�W�F�N�g��id
	std::list<std::weak_ptr<GameObject>> delete_objs_;
	boost::mutex delete_objs_mutex_;
	//���폜�\���Component�̃��X�g
	std::list<std::weak_ptr<Component>> delete_comps_;
	boost::mutex delete_comps_mutex_;
};

template<class T, class ...Args>
inline ComponentHandle<T> ElementContainer::AddUpdateComponent(std::shared_ptr<GameObject> obj, Args ...args)
{
	GameObjectHandle handle(obj);
	std::shared_ptr<T> shp = AllocateComponentInPool<T>(handle, args...);
	shp->SetSharedPtr(shp);
	{
		boost::unique_lock<boost::mutex> lock(update_comps_to_initiate_mutex_);
		update_comps_to_initiate_.push_back(shp);
	}
	{
		boost::unique_lock<boost::mutex> lock(comp_init_thread_func_mutex_);
		comp_init_thread_func_cond_.notify_one();
	}
	return ComponentHandle<T>(shp);
}

template<class T, class ...Args>
inline ComponentHandle<T> ElementContainer::AddOutputComponent(std::shared_ptr<GameObject> obj, Args ...args)
{
	GameObjectHandle handle(obj);
	std::shared_ptr<T> shp = AllocateComponentInPool<T>(handle, args...);
	shp->SetSharedPtr(shp);
	{
		boost::unique_lock<boost::mutex> lock(output_comps_to_initiate_mutex_);
		output_comps_to_initiate_.push_back(shp);
	}
	{
		boost::unique_lock<boost::mutex> lock(comp_init_thread_func_mutex_);
		comp_init_thread_func_cond_.notify_one();
	}
	return ComponentHandle<T>(shp);
}

template<class T, class ...Args>
inline std::shared_ptr<T> ElementContainer::AllocateComponentInPool(GameObjectHandle obj, Args ...args)
{
	std::shared_ptr<T> shp;
	switch (GetSizeClass(sizeof(T))) {
	case 1: {
		boost::unique_lock<boost::mutex> lock(comp_pool_64_mutex_);
		if (comp_pool_used_chunk_64_ >= kMaxCompNum64_) {
			Log::OutputCritical("Component number in comp_pool_64_ exceeded limit");
		}
		comp_pool_used_chunk_64_++;
		shp = std::shared_ptr<T>(new(comp_pool_64_->malloc()) T(obj, args...), CompPoolDeleter64);
		break;
	}
	case 2: {
		boost::unique_lock<boost::mutex> lock(comp_pool_96_mutex_);
		if (comp_pool_used_chunk_96_ >= kMaxCompNum96_) {
			Log::OutputCritical("Component number in comp_pool_96_ exceeded limit");
		}
		comp_pool_used_chunk_96_++;
		shp = std::shared_ptr<T>(new(comp_pool_96_->malloc()) T(obj, args...), CompPoolDeleter96);
		break;
	}
	case 3: {
		boost::unique_lock<boost::mutex> lock(comp_pool_128_mutex_);
		if (comp_pool_used_chunk_128_ >= kMaxCompNum128_) {
			Log::OutputCritical("Component number in comp_pool_128_ exceeded limit");
		}
		comp_pool_used_chunk_128_++;
		shp = std::shared_ptr<T>(new(comp_pool_128_->malloc()) T(obj, args...), CompPoolDeleter128);
		break;
	}
	case 4: {
		shp = std::shared_ptr<T>(new T(obj, args...));
		break;
	}
	}
	return shp;
}

