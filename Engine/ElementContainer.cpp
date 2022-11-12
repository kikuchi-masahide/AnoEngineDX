//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ElementContainer.h"

#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include <stack>
#include "Log.h"

void ElementContainer::MemoryInit()
{
	obj_pool_.emplace(sizeof(GameObject), kMaxObjNum_);
	comp_pool_64_.emplace(64, kMaxCompNum64_);
	comp_pool_96_.emplace(96, kMaxCompNum96_);
	comp_pool_128_.emplace(128, kMaxCompNum128_);
}

ElementContainer::ElementContainer()
{
	CreateCompInitThread();
}

ElementContainer::~ElementContainer()
{
	FinishCompInitThread();
}

void ElementContainer::LaunchUpdateComponents()
{
	for (auto ptr : update_components_) {
		ptr->Update();
	}
}

void ElementContainer::LaunchOutputComponents()
{
	if (output_components_.size() > 0) {
		//直前に実行したOutputComponentのupd_pirority_
		int prev_upd_prio = output_components_[0]->upd_priority_;
		auto itr = output_func_in_.find(prev_upd_prio);
		if (itr != output_func_in_.end()) {
			(itr->second)();
		}
		for (auto ptr : output_components_) {
			if (ptr->upd_priority_ != prev_upd_prio) {
				itr = output_func_out_.find(prev_upd_prio);
				if (itr != output_func_out_.end()) {
					//登録した関数を実行
					(itr->second)();
				}
				itr = output_func_in_.find(ptr->upd_priority_);
				if (itr != output_func_in_.end()) {
					(itr->second)();
				}
				prev_upd_prio = ptr->upd_priority_;
			}
			ptr->Update();
		}
		itr = output_func_out_.find(prev_upd_prio);
		if (itr != output_func_out_.end()) {
			(itr->second)();
		}
	}
}

void ElementContainer::ProcessPandingElements()
{
	//子Componentが消去されたのでそのリストを変更する必要があるオブジェクト(DeleteFlagがonの可能性あり)
	std::queue<std::weak_ptr<GameObject>> obj_to_reset_childs;
	while (true) {
		//一回もコンポーネント削除を行わなければtrue
		bool no_delete = true;
		std::list<std::weak_ptr<Component>> compswap;
		{
			boost::unique_lock<boost::mutex> lock(delete_comps_mutex_);
			compswap.swap(delete_comps_);
		}
		while (!compswap.empty()) {
			no_delete = false;
			auto shptr = compswap.front().lock();
			obj_to_reset_childs.push(shptr->obj_);
			shptr->CleanUp();
			compswap.pop_front();
		}
		{
			boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
			if (update_comps_to_initiate_.empty() && output_comps_to_initiate_.empty()) {
				boost::unique_lock<boost::mutex> dlock(delete_comps_mutex_);
				if (delete_comps_.empty()) {
					break;
				}
			}
			else {
				//AsyncInitializeの完了を待つ
				//HACK:多分これが一番安全だけど、もっと速い方法がある気がする なんとなくだけど
				initiate_comps_cv_.wait(lock);
			}
		}
	}
	boost::thread thread_update(&ElementContainer::MergeUpdateComponents, this);
	boost::thread thread_output(&ElementContainer::MergeOutputComponents, this);
	std::erase_if(objs_, [](const std::shared_ptr<GameObject>& ptr) {
		return ptr->GetDeleteFlag();
	});
	while (!obj_to_reset_childs.empty()) {
		auto obj = obj_to_reset_childs.front();
		if (!obj.expired()) {
			obj.lock()->UnregisterInvalidChilds();
		}
		obj_to_reset_childs.pop();
	}
	thread_update.join();
	thread_output.join();
}

std::weak_ptr<GameObject> ElementContainer::AddObject(Scene* scene)
{
	std::shared_ptr<GameObject> shp;
	{
		if (obj_pool_used_chunk_ >= kMaxObjNum_) {
			Log::OutputCritical("Object number exceeded limit");
		}
		boost::unique_lock<boost::mutex> lock(obj_pool_mutex_);
		shp = std::shared_ptr<GameObject>(new(obj_pool_->malloc()) GameObject(scene), ObjPoolDeleter);
		obj_pool_used_chunk_++;
	}
	shp->SetSharedPtr(shp);
	objs_.push_back(shp);
	return shp;
}

void ElementContainer::SetOutputCompsPreFunc(int upd_prio, std::function<void()> func)
{
	//HACK:変なこと変なタイミングするとバグる
	output_func_in_[upd_prio] = func;
}

void ElementContainer::SetOutputCompsPostFunc(int upd_prio, std::function<void()> func)
{
	output_func_out_[upd_prio] = func;
}

void ElementContainer::Erase(std::weak_ptr<Component> ptr)
{
	boost::unique_lock<boost::mutex> lock(delete_comps_mutex_);
	delete_comps_.push_back(ptr);
}

void ElementContainer::FreeAllElements()
{
	for (auto& comp : update_components_) {
		comp->CleanUp();
	}
	update_components_.clear();
	for (auto& comp : panding_update_components_) {
		comp->CleanUp();
	}
	panding_update_components_.clear();
	for (auto& comp : output_components_) {
		comp->CleanUp();
	}
	output_components_.clear();
	for (auto& comp : panding_output_components_) {
		comp->CleanUp();
	}
	panding_output_components_.clear();
	objs_.clear();
}

int ElementContainer::GetGameObjectNumber() const
{
	return objs_.size();
}

int ElementContainer::GetUpdateComponentNumber() const
{
	return update_components_.size();
}

int ElementContainer::GetOutputComponentNumber() const
{
	return output_components_.size();
}

void ElementContainer::CreateCompInitThread()
{
	boost::thread th(&ElementContainer::CompInitThreadFunc, this);
	comp_init_thread_.swap(th);
}

void ElementContainer::FinishCompInitThread()
{
	//AsyncInitializeスレッドに中断を通知
	comp_init_thread_.interrupt();
	comp_init_thread_.join();
}

int ElementContainer::GetSizeClass(std::size_t size)
{
	if (size <= 64) {
		return 1;
	}
	else if (size <= 96) {
		return 2;
	}
	else if (size <= 128) {
		return 3;
	}
	else {
		return 4;
	}
}

void ElementContainer::ObjPoolDeleter(GameObject* p)
{
	p->~GameObject();
	{
		boost::unique_lock<boost::mutex> lock(obj_pool_mutex_);
		obj_pool_->free(p);
		obj_pool_used_chunk_--;
	}
}

void ElementContainer::CompPoolDeleter64(Component* p)
{
	p->~Component();
	{
		boost::unique_lock<boost::mutex> lock(comp_pool_64_mutex_);
		comp_pool_64_->free(p);
		comp_pool_used_chunk_64_--;
	}
}

void ElementContainer::CompPoolDeleter96(Component* p)
{
	p->~Component();
	{
		boost::unique_lock<boost::mutex> lock(comp_pool_96_mutex_);
		comp_pool_96_->free(p);
		comp_pool_used_chunk_96_--;
	}
}

void ElementContainer::CompPoolDeleter128(Component* p)
{
	p->~Component();
	{
		boost::unique_lock<boost::mutex> lock(comp_pool_128_mutex_);
		comp_pool_128_->free(p);
		comp_pool_used_chunk_128_--;
	}
}

void ElementContainer::CompInitThreadFunc()
{
	//スレッドの終了命令が飛んでいても、全AsyncInitializeは終わらせる
	while (true) {
		//次AsyncInitializeするコンポーネント
		std::shared_ptr<Component> cmp;
		{
			boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
			if (!update_comps_to_initiate_.empty()) {
				cmp = update_comps_to_initiate_.front();
			}
			else {
				cmp = nullptr;
			}
		}
		while (cmp) {
			cmp->AsyncInitialize();
			panding_update_components_.push_back(cmp);
			//AsyncInitializeを完了するまでpopはしない
			boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
			update_comps_to_initiate_.pop();
			if (!update_comps_to_initiate_.empty()) {
				cmp = update_comps_to_initiate_.front();
			}
			else {
				cmp = nullptr;
			}
		}
		{
			boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
			if (!output_comps_to_initiate_.empty()) {
				cmp = output_comps_to_initiate_.front();
			}
			else {
				cmp = nullptr;
			}
		}
		while (cmp) {
			cmp->AsyncInitialize();
			panding_output_components_.push_back(cmp);
			//AsyncInitializeを完了するまでpopはしない
			boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
			output_comps_to_initiate_.pop();
			if (!output_comps_to_initiate_.empty()) {
				cmp = output_comps_to_initiate_.front();
			}
			else {
				cmp = nullptr;
			}
		}
		{
			boost::unique_lock<boost::mutex> lock(comps_to_initiate_mutex_);
			if (update_comps_to_initiate_.empty() && output_comps_to_initiate_.empty()) {
				//update_comps_to_initiate_/output_comps_to_initiate_の変更を通知
				initiate_comps_cv_.notify_all();
				//update_comps_to_initiate_/output_comps_to_initiate_が変更されるとwaitが切れる
				//このスレッドがinterruptされた場合、このwaitでスレッド終了する
				initiate_comps_cv_.wait(lock);
			}
		}
	}//AsyncInitializeのループ
}

void ElementContainer::MergeUpdateComponents()
{
	std::erase_if(update_components_, [](const std::shared_ptr<Component>& wkptr) {
		return wkptr->GetDeleteFlag();
	});
	std::erase_if(panding_update_components_, [](const std::shared_ptr<Component>& wkptr) {
		return wkptr->GetDeleteFlag();
	});
	update_components_.reserve(update_components_.size() + panding_update_components_.size());
	std::copy(panding_update_components_.begin(), panding_update_components_.end(),
		std::back_inserter(update_components_));
	panding_update_components_.clear();
	//HACK:listにして挿入する感じでいけばソートがO(N)になる?
	std::sort(update_components_.begin(), update_components_.end(),
		[](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
		return a->upd_priority_ < b->upd_priority_;
	});
}

void ElementContainer::MergeOutputComponents()
{
	std::erase_if(output_components_, [](const std::shared_ptr<Component>& wkptr) {
		return wkptr->GetDeleteFlag();
	});
	std::erase_if(panding_output_components_, [](const std::shared_ptr<Component>& wkptr) {
		return wkptr->GetDeleteFlag();
	});
	output_components_.reserve(output_components_.size() + panding_output_components_.size());
	std::copy(panding_output_components_.begin(), panding_output_components_.end(),
		std::back_inserter(output_components_));
	panding_output_components_.clear();
	std::sort(output_components_.begin(), output_components_.end(),
		[](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
		return a->upd_priority_ < b->upd_priority_;
	});
}

std::optional<boost::pool<>> ElementContainer::obj_pool_;
boost::mutex ElementContainer::obj_pool_mutex_;
int ElementContainer::obj_pool_used_chunk_ = 0;
std::optional<boost::pool<>> ElementContainer::comp_pool_64_;
std::optional<boost::pool<>> ElementContainer::comp_pool_96_;
std::optional<boost::pool<>> ElementContainer::comp_pool_128_;
boost::mutex ElementContainer::comp_pool_64_mutex_;
boost::mutex ElementContainer::comp_pool_96_mutex_;
boost::mutex ElementContainer::comp_pool_128_mutex_;
int ElementContainer::comp_pool_used_chunk_64_ = 0;
int ElementContainer::comp_pool_used_chunk_96_ = 0;
int ElementContainer::comp_pool_used_chunk_128_ = 0;
