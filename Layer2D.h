#pragma once
#include "Layer.h"
#include "Game.h"
#include "ComponentHandle.h"

class Layer2DComponent;

class Layer2D :public Layer {
public:
	Layer2D(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap);
	~Layer2D();
	virtual void Draw();
	void InsertComponent(ComponentHandle<Layer2DComponent> _comp);
private:
	boost::shared_ptr<DX12Resource> mVertResource;
	boost::shared_ptr<DX12Resource> mIndexResource;
	boost::shared_ptr<DX12ShaderObject> mVS, mPS;
	boost::shared_ptr<DX12GraphicsPipeline> mPipeline;
	boost::shared_ptr<DX12RootSignature> mRootSignature;
	boost::shared_ptr<DX12Resource> mTexReadResource;
	boost::shared_ptr<DX12Resource> mBackbuffer;
	boost::shared_ptr<DX12DescriptorHeap> mRTV;
	boost::shared_ptr<DX12DescriptorHeap> mSRV;
	DX12VertexLayout mVertLayout;
	struct VertexLayout {
	public:
		Vector3 mPos;
		Vector2 mUV;
	};
	void GraphicalInit();
	//Layer2DCoimponentÇzç~èáÇ≈åƒÇ—ÇæÇ∑
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle<Layer2DComponent>& left, const ComponentHandle<Layer2DComponent>& right) const;
	};
	std::set<ComponentHandle<Layer2DComponent>,ComponentHandleCompare> mComps;
};