//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Log.h"

class Scene;
class GameObject;
class Component;

//Sceneで用いる、Componentのコンテナや関連処理をまとめたクラス
class ElementContainer
{
public:
	//初期プールに同時に保存できるオブジェクトの最大数
	static constexpr int kMaxObjNum_ = 10000;
	//各初期プールに同時に保存できるComponentの最大数
	static constexpr int kMaxCompNum64_ = 20000;
	static constexpr int kMaxCompNum96_ = 10000;
	static constexpr int kMaxCompNum128_ = 10000;
	static void MemoryInit();
	ElementContainer();
	~ElementContainer();
	//このContainerに関数実行時点で含まれる、UpdateComponentのUpdateを順次実行する
	void LaunchUpdateComponents();
	//このContainerのOutputComponentのUpdateを順次実行する
	void LaunchOutputComponents();
	//このフレーム内で生成されたComponentのInitialize実行および不要なComponentを削除する
	void ProcessPandingElements();
	//メモリプールにオブジェクトを確保
	std::weak_ptr<GameObject> AddObject(Scene* scene);
	//objの指すGameObjectにUpdateComponentを追加(Scene::AddUpdateComponentから呼び出される)
	template<class T, class... Args>
	std::weak_ptr<T> AddUpdateComponent(std::weak_ptr<GameObject> obj, Args... args);
	//objの指すGameObjectにOutputComponentを追加(Scene::AddOutputComponentから呼び出される)
	template<class T, class... Args>
	std::weak_ptr<T> AddOutputComponent(std::weak_ptr<GameObject> obj, Args... args);
	//このupd_prioを持つコンポーネントを実行する前に実行する関数を登録する
	//(コンポーネントが存在しなければ実行しない)
	void SetOutputCompsPreFunc(int upd_prio,std::function<void()> func);
	//このupd_prioを持つ最後のコンポーネントを実行した後に実行する関数を登録する
	//(コンポーネントが存在しなければ実行しない)
	void SetOutputCompsPostFunc(int upd_prio, std::function<void()> func);
	//このComponentを、今フレームOutput後に削除する(Scene::Eraseから呼び出される)
	void Erase(std::weak_ptr<Component> ptr);
	//このContainerのもつGameObject/Componentをすべて解放する
	void FreeAllElements();
	int GetGameObjectNumber() const;
	int GetUpdateComponentNumber() const;
	int GetOutputComponentNumber() const;
private:
	//Component::Initを呼び出すための専用スレッドを立てる
	void CreateCompInitThread();
	//現在溜まっているCompのInitが終わったらInitスレッドを終了させ、スレッド終了のタイミングで処理を返す
	void FinishCompInitThread();
	//渡されたサイズから、Componentを格納すべきプールを決定する
	static int GetSizeClass(std::size_t size);
	//GameObjectを保存するメモリプール
	//HACK:メモリプールのアロケータをDBG_NEWにできればリークの時にこの行数が出力できるが、
	//そもそも通常のnewでもリーク自体は検出できているので、とりあえず今は考えていない
	static std::optional<boost::pool<>> obj_pool_;
	//std::shared_ptr<GameObject>用のdeleter
	static void ObjPoolDeleter(GameObject* p);
	static boost::mutex obj_pool_mutex_;
	//obj_pool_の、使用チャンク数
	static int obj_pool_used_chunk_;
	//チャンクサイズが64,96,128バイトであるComponent用プール
	static std::optional<boost::pool<>> comp_pool_64_;
	static std::optional<boost::pool<>> comp_pool_96_;
	static std::optional<boost::pool<>> comp_pool_128_;
	//各メモリプールから取得したメモリに対するdeleter(std::shared_ptr用)
	static void CompPoolDeleter64(Component* p);
	static void CompPoolDeleter96(Component* p);
	static void CompPoolDeleter128(Component* p);
	static boost::mutex comp_pool_64_mutex_;
	static boost::mutex comp_pool_96_mutex_;
	static boost::mutex comp_pool_128_mutex_;
	//componentプールの、使用チャンク数
	static int comp_pool_used_chunk_64_;
	static int comp_pool_used_chunk_96_;
	static int comp_pool_used_chunk_128_;
	//メモリプール上にアロケートしコンストラクタを実行する Scene::AddUpdate/OutputComponent用
	template<class T, class... Args>
	std::shared_ptr<T> AllocateComponentInPool(std::weak_ptr<GameObject> obj, Args... args);
	//Update中にComponentのInitialize()を別スレッドで実行する関数
	void CompInitThreadFunc();
	//update_components_とpanding_update_components_を整理し融合する(マルチスレッド用)
	void MergeUpdateComponents();
	//output_components_とpanding_output_components_を整理し融合する(マルチスレッド用)
	void MergeOutputComponents();
	std::vector<std::shared_ptr<GameObject>> objs_;
	//自身の持つ更新・出力コンポーネントのリスト，および保留コンポーネント
	//HACK:余裕あったら別のコンテナに変えた場合のパフォーマンス比較
	//update_components_は
	//・LaunchUpdateComponentsでイテレートされ、
	//・ProcessPandingElementsでpanding_update_components_と結合される
	//が、二つが同時に起こることはないため、mutexは必要ない
	std::vector<std::shared_ptr<Component>> update_components_;
	//panding_update_components_は
	//・CompInitThreadFuncで追加され
	//・ProcessPandingElementsでupdate_components_と結合される
	//が、AsyncInitializeはすべてProcessPandingElementsの前に完了するのでmutexは必要ない
	std::vector<std::shared_ptr<Component>> panding_update_components_;
	std::vector<std::shared_ptr<Component>> output_components_;
	//このupd_priority_を持つOutputComponentのUpdateをはじめて実行する前に、このmapに登録した関数を呼び出す
	std::map<int, std::function<void()>> output_func_in_;
	//このupd_priority_を持つ最後のOutputComponentのUpdateを実行する前に、このmapに登録した関数を呼び出す
	std::map<int, std::function<void()>> output_func_out_;
	std::vector<std::shared_ptr<Component>> panding_output_components_;
	//Update中にComponentのInitialize()を実行するためのスレッド(CreateCompInitThreadInUpdate()で作成)
	boost::thread comp_init_thread_;
	//Initiateを実行するべきComponentたち 後から追加し、前から実行していく
	std::queue<std::shared_ptr<Component>> update_comps_to_initiate_;
	std::queue<std::shared_ptr<Component>> output_comps_to_initiate_;
	//udpate/output_comps_to_initiate_両方に対するmutex
	boost::mutex comps_to_initiate_mutex_;
	//update_comps_to_initiate_とoutput_comps_to_initiate_の変更を通知するcondition_variable
	boost::condition_variable initiate_comps_cv_;
	//次消すべきオブジェクトのid
	std::list<std::weak_ptr<GameObject>> delete_objs_;
	boost::mutex delete_objs_mutex_;
	//次削除予定のComponentのリスト(LIFO)
	std::list<std::weak_ptr<Component>> delete_comps_;
	boost::mutex delete_comps_mutex_;
};

template<class T, class ...Args>
inline std::weak_ptr<T> ElementContainer::AddUpdateComponent(std::weak_ptr<GameObject> obj, Args ...args)
{
	std::shared_ptr<T> shp = AllocateComponentInPool<T>(obj, args...);
	shp->SetSharedPtr(shp);
	{
		boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
		update_comps_to_initiate_.push(shp);
		//別スレッドに変更を通知
		initiate_comps_cv_.notify_all();
	}
	return shp;
}

template<class T, class ...Args>
inline std::weak_ptr<T> ElementContainer::AddOutputComponent(std::weak_ptr<GameObject> obj, Args ...args)
{
	std::shared_ptr<T> shp = AllocateComponentInPool<T>(obj, args...);
	shp->SetSharedPtr(shp);
	{
		boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
		output_comps_to_initiate_.push(shp);
		//別スレッドに変更を通知
		initiate_comps_cv_.notify_all();
	}
	return shp;
}

template<class T, class ...Args>
inline std::shared_ptr<T> ElementContainer::AllocateComponentInPool(std::weak_ptr<GameObject> obj, Args ...args)
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

