//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ElementContainer.h"

#include "Component.h"
#include "GameObject.h"

void ElementContainer::MemoryInit()
{
	obj_pool_.emplace(sizeof(GameObject), kMaxObjNum_);
	comp_pool_64_.emplace(64, kMaxCompNum64_);
	comp_pool_96_.emplace(96, kMaxCompNum96_);
	comp_pool_128_.emplace(128, kMaxCompNum128_);
}

ElementContainer::ElementContainer()
{
}

ElementContainer::~ElementContainer()
{
}

void ElementContainer::CreateCompInitThread()
{
	comp_init_thread_flag_ = true;
	boost::thread th(&ElementContainer::CompInitThreadFunc, this);
	comp_init_thread_.swap(th);
}

void ElementContainer::LaunchUpdateComponents()
{
	for (auto wkptr : update_components_) {
		wkptr.lock()->Update();
	}
}

void ElementContainer::LaunchOutputComponents()
{
	if (output_components_.size() > 0) {
		//直前に実行したOutputComponentのupd_pirority_
		int prev_upd_prio = output_components_[0].lock()->upd_priority_;
		auto itr = output_func_in_.find(prev_upd_prio);
		if (itr != output_func_in_.end()) {
			(itr->second)();
		}
		for (auto wkptr : output_components_) {
			auto shptr = wkptr.lock();
			if (shptr->upd_priority_ != prev_upd_prio) {
				itr = output_func_out_.find(prev_upd_prio);
				if (itr != output_func_out_.end()) {
					//登録した関数を実行
					(itr->second)();
				}
				itr = output_func_in_.find(shptr->upd_priority_);
				if (itr != output_func_in_.end()) {
					(itr->second)();
				}
				prev_upd_prio = shptr->upd_priority_;
			}
			shptr->Update();
		}
		itr = output_func_out_.find(prev_upd_prio);
		if (itr != output_func_out_.end()) {
			(itr->second)();
		}
	}
}

void ElementContainer::FinishCompInitThread()
{
	{
		boost::unique_lock<boost::mutex> lock(comp_init_thread_func_mutex_);
		comp_init_thread_flag_ = false;
		comp_init_thread_func_cond_.notify_one();
	}
	comp_init_thread_.join();
}

void ElementContainer::ProcessPandingElements()
{
	//HACK:マルチスレッド化できるか?
	std::set<GameObjectHandle> obj_to_reset_childs;
	while (true) {
		if (!delete_comps_.empty()) {
			auto shptr = delete_comps_.front().lock();
			obj_to_reset_childs.insert(shptr->obj_);
			shptr->ResetSharedPtr(shptr);
			delete_comps_.pop_front();
			continue;
		}
		else if (!update_comps_to_initiate_.empty()) {
			auto shptr = update_comps_to_initiate_.front().lock();
			shptr->AsyncInitialize();
			panding_update_components_.push_back(shptr);
			update_comps_to_initiate_.pop_front();
			continue;
		}
		else if (!output_comps_to_initiate_.empty()) {
			auto shptr = output_comps_to_initiate_.front().lock();
			shptr->AsyncInitialize();
			panding_output_components_.push_back(shptr);
			output_comps_to_initiate_.pop_front();
			continue;
		}
		else if (!delete_objs_.empty()) {
			auto shptr = delete_objs_.front().lock();
			shptr->ResetSharedPtr(shptr);
			delete_objs_.pop_front();
			continue;
		}
		else {
			break;
		}
	}
	for (auto obj : obj_to_reset_childs) {
		if (obj) {
			obj->UnregisterInvalidChilds();
		}
	}
	boost::thread thread_update(&ElementContainer::MergeUpdateComponents, this);
	boost::thread thread_output(&ElementContainer::MergeOutputComponents, this);
	for (auto obj : obj_to_reset_childs) {
		if (obj) {
			obj->UnregisterInvalidChilds();
		}
	}
	obj_to_reset_childs.clear();
	std::erase_if(objs_, [](const std::weak_ptr<GameObject>& wkptr) {
		return wkptr.expired();
	});
	thread_update.join();
	thread_output.join();
}

GameObjectHandle ElementContainer::AddObject(Scene* scene)
{
	std::shared_ptr<GameObject> shp;
	{
		if (obj_pool_used_chunk_ >= kMaxObjNum_) {
			Log::OutputCritical("Object number exceeded limit");
		}
		boost::unique_lock<boost::mutex> lock(obj_pool_mutex_);
		//HACK:make_sharedを使う(アロケータが必要?)
		shp = std::shared_ptr<GameObject>(new(obj_pool_->malloc()) GameObject(scene), ObjPoolDeleter);
		obj_pool_used_chunk_++;
	}
	shp->SetSharedPtr(shp);
	objs_.push_back(shp);
	return GameObjectHandle(shp);
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

void ElementContainer::Erase(std::weak_ptr<GameObject> ptr)
{
	boost::unique_lock<boost::mutex> lock(delete_objs_mutex_);
	delete_objs_.push_back(ptr);
}

void ElementContainer::Erase(std::weak_ptr<Component> ptr)
{
	boost::unique_lock<boost::mutex> lock(delete_comps_mutex_);
	delete_comps_.push_back(ptr);
}

void ElementContainer::FreeAllElements()
{
	for (auto& comp : update_components_) {
		auto shp = comp.lock();
		shp->ResetSharedPtr(shp);
		shp.reset();
	}
	update_components_.clear();
	for (auto& comp : panding_update_components_) {
		auto shp = comp.lock();
		shp->ResetSharedPtr(shp);
		shp.reset();
	}
	panding_update_components_.clear();
	for (auto& comp : output_components_) {
		auto shp = comp.lock();
		shp->ResetSharedPtr(shp);
		shp.reset();
	}
	output_components_.clear();
	for (auto& comp : panding_output_components_) {
		auto shp = comp.lock();
		shp->ResetSharedPtr(shp);
		shp.reset();
	}
	panding_output_components_.clear();
	for (auto ptr : objs_) {
		std::shared_ptr<GameObject> shptr = ptr.lock();
		shptr->ResetSharedPtr(shptr);
		shptr.reset();
	}
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
	do {
		int size = 0;
		{
			boost::unique_lock<boost::mutex> lock(update_comps_to_initiate_mutex_);
			size = update_comps_to_initiate_.size();
		}
		if (size > 0) {
			//一時コピー先
			std::list<std::weak_ptr<Component>> cmps;
			{
				boost::unique_lock<boost::mutex> lock(update_comps_to_initiate_mutex_);
				cmps.swap(update_comps_to_initiate_);
			}
			for (auto wkptr : cmps) {
				wkptr.lock()->AsyncInitialize();
				panding_update_components_.push_back(wkptr);
			}
		}
		{
			boost::unique_lock<boost::mutex> lock(output_comps_to_initiate_mutex_);
			size = output_comps_to_initiate_.size();
		}
		if (size > 0) {
			//一時コピー先
			std::list<std::weak_ptr<Component>> cmps;
			{
				boost::unique_lock<boost::mutex> lock(output_comps_to_initiate_mutex_);
				cmps.swap(output_comps_to_initiate_);
			}
			for (auto wkptr : cmps) {
				wkptr.lock()->AsyncInitialize();
				panding_output_components_.push_back(wkptr);
			}
		}
		if (!comp_init_thread_flag_) {
			break;
		}
		{
			boost::unique_lock<boost::mutex> lock(comp_init_thread_func_mutex_);
			comp_init_thread_func_cond_.wait(lock);
		}
	} while (true);
}

void ElementContainer::MergeUpdateComponents()
{
	std::erase_if(update_components_, [](const std::weak_ptr<Component>& wkptr) {
		return wkptr.expired();
	});
	std::erase_if(panding_update_components_, [](const std::weak_ptr<Component>& wkptr) {
		return wkptr.expired();
	});
	update_components_.reserve(update_components_.size() + panding_update_components_.size());
	std::copy(panding_update_components_.begin(), panding_update_components_.end(),
		std::back_inserter(update_components_));
	panding_update_components_.clear();
	//HACK:listにして挿入する感じでいけばソートがO(N)になる?
	std::sort(update_components_.begin(), update_components_.end(),
		[](const std::weak_ptr<Component>& a, const std::weak_ptr<Component>& b) {
		return a.lock()->upd_priority_ < b.lock()->upd_priority_;
	});
}

void ElementContainer::MergeOutputComponents()
{
	std::erase_if(output_components_, [](const std::weak_ptr<Component>& wkptr) {
		return wkptr.expired();
	});
	std::erase_if(panding_output_components_, [](const std::weak_ptr<Component>& wkptr) {
		return wkptr.expired();
	});
	output_components_.reserve(output_components_.size() + panding_output_components_.size());
	std::copy(panding_output_components_.begin(), panding_output_components_.end(),
		std::back_inserter(output_components_));
	panding_output_components_.clear();
	std::sort(output_components_.begin(), output_components_.end(),
		[](const std::weak_ptr<Component>& a, const std::weak_ptr<Component>& b) {
		return a.lock()->upd_priority_ < b.lock()->upd_priority_;
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
