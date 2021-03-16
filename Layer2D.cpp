#include "Layer2D.h"
#include "Layer2DComponent.h"
#include "window.h"

Layer2D::Layer2D(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap)
	:Layer(_scene,_hset,_rect,_z,_swap)
{
	GraphicalInit();
}

Layer2D::~Layer2D()
{
}

void Layer2D::Draw()
{
	//自分の持つバックバッファをレンダーターゲットに
	GetGame().mdx12.SetResourceBarrier(mBackbuffer, DX12Config::ResourceBarrierState::PIXEL_SHADER_RESOURCE, DX12Config::ResourceBarrierState::RENDER_TARGET);
	GetGame().mdx12.OpenRenderTarget(mRTV, 0);
	GetGame().mdx12.ClearRenderTarget(mRTV, 0, 1.0f, 1.0f, 1.0f, 1.0f);
	for (auto itr = mComps.begin(); itr != mComps.end(); itr++)
	{
		(*itr)->Draw();
	}
	mComps.clear();
	//backbufferをテクスチャに戻す
	GetGame().mdx12.SetResourceBarrier(mBackbuffer, DX12Config::ResourceBarrierState::RENDER_TARGET, DX12Config::ResourceBarrierState::PIXEL_SHADER_RESOURCE);
	//_swapchainをレンダーターゲットに
	GetGame().OpenSwapChain(GetSwapchainID());
	//頂点バッファの設定
	float* map = (float*)GetGame().mdx12.Map(mVertResource);
	auto rect = GetRect();
	auto clrect = GetGame().GetWindow(GetSwapchainID())->GetWindowSize();
	auto clwidth = clrect.right - clrect.left;
	auto clheight = clrect.bottom - clrect.top;
	float xl = rect.GetLD()(0);
	float xr = rect.GetRU()(0);
	float yu = rect.GetRU()(1);
	float yb = rect.GetLD()(1);
	xl = 2 * xl / clwidth - 1;
	xr = 2 * xr / clwidth - 1;
	yu = 2 * yu / clheight - 1;
	yb = 2 * yb / clheight - 1;
	map[0] = xl;
	map[1] = yb;
	map[5] = xl;
	map[6] = yu;
	map[10] = xr;
	map[11] = yb;
	map[15] = xr;
	map[16] = yu;
	GetGame().mdx12.SetGraphicsPipeline(mPipeline);
	GetGame().mdx12.SetRootSignature(mRootSignature);
	GetGame().mdx12.SetDescriptorHeap(mSRV);
	GetGame().mdx12.SetGraphicsRootDescriptorTable(0, mSRV, 0);
	GetGame().mdx12.SetPrimitiveTopology(DX12Config::PrimitiveTopology::TRIANGLESTRIP);
	GetGame().mdx12.SetVertexBuffers(mVertResource, 0, sizeof(float) * 5 * 4, sizeof(float) * 5);
	GetGame().mdx12.SetIndexBuffers(mIndexResource, 6);
	GetGame().mdx12.SetViewports(clwidth, clheight, 0.0f,0.0f, 1.0f, 0.0f);
	GetGame().mdx12.SetScissorrect(0,clheight, 0,clwidth);
	GetGame().mdx12.DrawIndexedInstanced(6, 1, 0, 0, 0);
	GetGame().CloseSwapChain();
}

void Layer2D::GraphicalInit()
{
	VertexLayout vert[] = {
			{MatVec::Vector3(-1.0f,-1.0f,0.0f),MatVec::Vector2(0.0f,1.0f)},
			{MatVec::Vector3(-1.0f,1.0f,0.0f),MatVec::Vector2(0.0f,0.0f)},
			{MatVec::Vector3(1.0f,-1.0f,0.0f),MatVec::Vector2(1.0f,1.0f)},
			{MatVec::Vector3(1.0f,1.0f,0.0f),MatVec::Vector2(1.0f,0.0f)}
	};
	auto mVS = GetGame().mShaderManager.GetDX12ShaderObject(2);
	auto mPS = GetGame().mShaderManager.GetDX12ShaderObject(3);
	mVertResource = GetGame().mdx12.CreateVertexBuffer(sizeof(float) * 5 * 4);
	float* resmap = (float*)(GetGame().mdx12.Map(mVertResource));
	for (unsigned int n = 0; n < 4; n++) {
		resmap[5 * n] = vert[n].mPos(0);
		resmap[5 * n + 1] = vert[n].mPos(1);
		resmap[5 * n + 2] = vert[n].mPos(2);
		resmap[5 * n + 3] = vert[n].mUV(0);
		resmap[5 * n + 4] = vert[n].mUV(1);
	}
	GetGame().mdx12.Unmap(mVertResource);
	mVertLayout.push_back(DX12VertexLayoutUnit(
		"POSITION", DX12Config::VertexLayoutFormat::R32G32B32_FLOAT, 0,
		DX12Config::VertexLayoutInputClassification::INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	));
	mVertLayout.push_back(DX12VertexLayoutUnit(
		"TEXCOORD", DX12Config::VertexLayoutFormat::R32G32_FLOAT, 0,
		DX12Config::VertexLayoutInputClassification::INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	));
	DX12DescriptorRange range1(1, DX12Config::DescriptorRangeType::SRV, 0);
	DX12RootParameter rootparam;
	rootparam.mShaderVisibility = DX12Config::RootParameterShaderVisibility::ALL;
	rootparam.mDescRanges.push_back(range1);
	mRootSignature = GetGame().mdx12.CreateRootSignature(rootparam);
	mPipeline = GetGame().mdx12.CreateGraphicsPipeline(
		mVS, mPS, mVertLayout, DX12Config::PrimitiveTopologyType::TRIANGLE, 1, mRootSignature
	);
	unsigned int indeces[] = {
		0,1,2,
		2,1,3
	};
	mIndexResource = GetGame().mdx12.CreateIndexBuffer(6);
	unsigned int* indresmap = (unsigned int*)GetGame().mdx12.Map(mIndexResource);
	for (unsigned int n = 0; n < 6; n++) {
		indresmap[n] = indeces[n];
	}
	GetGame().mdx12.Unmap(mIndexResource);
	//バックバッファとRTV，SRVの作成
	auto rect = GetRect();
	mBackbuffer = GetGame().mdx12.CreateClearTexture(rect.GetWidth(), rect.GetHeight(), 1.0f, 1.0f, 1.0f, 1.0f);
	mRTV = GetGame().mdx12.CreateDescriptorHeap(DX12Config::DescriptorHeapType::RTV, DX12Config::DescriptorHeapShaderVisibility::NONE, 1);
	mSRV = GetGame().mdx12.CreateDescriptorHeap(DX12Config::DescriptorHeapType::SRV, DX12Config::DescriptorHeapShaderVisibility::SHADER_VISIBLE, 1);
	GetGame().mdx12.CreateRenderTargetView(mBackbuffer, mRTV, 0);
	GetGame().mdx12.CreateShaderResourceView(mBackbuffer, mSRV, 0);
}

void Layer2D::InsertComponent(ComponentHandle<Layer2DComponent> _comp)
{
	mComps.insert(_comp);
}

bool Layer2D::ComponentHandleCompare::operator()(const ComponentHandle<Layer2DComponent>& left, const ComponentHandle<Layer2DComponent>& right) const
{
	return left->z < right->z;
}
