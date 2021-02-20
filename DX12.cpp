#include "DX12.h"
#include "DX12Pimple.h"

DX12::DX12() {}

void DX12::Initialize() {
	mPimple = new DX12Pimple();
	mPimple->Initialize();
}

void DX12::CleanUp() {
	mPimple->CleanUp();
}

DX12::~DX12()
{
	delete mPimple;
}

boost::shared_ptr<DX12DescriptorHeap> DX12::CreateDescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::DescriptorHeapShaderVisibility _vis, unsigned int _num)
{
	return mPimple->CreateDescriptorHeap(_type, _vis, _num);
}

void DX12::ProcessCommands()
{
	mPimple->ProcessCommands();
}

boost::shared_ptr<DX12Resource> DX12::CreateVertexBuffer(UINT64 _width)
{
	return mPimple->CreateVertexBuffer(_width);
}

void* DX12::Map(boost::shared_ptr<DX12Resource> _resource) {
	return mPimple->Map(_resource);
}

void DX12::Unmap(boost::shared_ptr<DX12Resource> _resource) {
	return mPimple->Unmap(_resource);
}

boost::shared_ptr<DX12ShaderObject> DX12::LoadShader(LPCWSTR _filename, DX12Config::ShaderType _shaderType)
{
	return mPimple->LoadShader(_filename, _shaderType);
}

boost::shared_ptr<DX12GraphicsPipeline> DX12::CreateGraphicsPipeline(boost::shared_ptr<DX12ShaderObject> _vertexShader, boost::shared_ptr<DX12ShaderObject> _pixelShader, DX12VertexLayout& _vertexLayout, DX12Config::PrimitiveTopologyType _primitive, UINT _numrt, boost::shared_ptr<DX12RootSignature> _rootsignature)
{
	return mPimple->CreateGraphicsPipeline(_vertexShader, _pixelShader, _vertexLayout, _primitive, _numrt, _rootsignature);
}

boost::shared_ptr<DX12RootSignature> DX12::CreateRootSignature(DX12RootParameter& _rootparam)
{
	return mPimple->CreateRootSignature(_rootparam);
}

void DX12::SetGraphicsPipeline(boost::shared_ptr<DX12GraphicsPipeline> _pipeline)
{
	mPimple->SetGraphicsPipeline(_pipeline);
}

void DX12::SetRootSignature(boost::shared_ptr<DX12RootSignature> _root)
{
	mPimple->SetRootSignature(_root);
}

void DX12::SetPrimitiveTopology(DX12Config::PrimitiveTopology _prim)
{
	mPimple->SetPrimitiveTopology(_prim);
}

void DX12::SetVertexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _slotid, SIZE_T _allsize, SIZE_T _sizepervertex)
{
	mPimple->SetVertexBuffers(_resource, _slotid, _allsize, _sizepervertex);
}

void DX12::DrawIndexedInstanced(UINT _indexnumperinst, UINT _instnum, UINT _indoffset, UINT _vdataoffset, UINT _instoffset)
{
	mPimple->DrawIndexedInstanced(_indexnumperinst, _instnum, _indoffset, _vdataoffset, _instoffset);
}

void DX12::SetViewports(UINT _widthpx, UINT _heightpx, int _topleftx, int _toplefty, float _maxdepth, float _mindepth)
{
	mPimple->SetViewports(_widthpx, _heightpx, _topleftx, _toplefty,_maxdepth,_mindepth);
}

void DX12::SetScissorrect(float _top, float _bottom, float _left, float _right)
{
	mPimple->SetScissorrect(_top, _bottom, _left, _right);
}

void DX12::SetIndexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _vertnum)
{
	mPimple->SetIndexBuffers(_resource, _vertnum);
}

boost::shared_ptr<DX12Resource> DX12::CreateIndexBuffer(unsigned int _vertnum)
{
	return mPimple->CreateIndexBuffer(_vertnum);
}

boost::shared_ptr<DX12Resource> DX12::LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num)
{
	return mPimple->LoadTexture(_filename, _desc, _num);
}

void DX12::SetDescriptorHeap(boost::shared_ptr<DX12DescriptorHeap> _descHeap)
{
	mPimple->SetDescriptorHeap(_descHeap);
}

void DX12::SetGraphicsRootDescriptorTable(unsigned int _rootParamIndex, boost::shared_ptr<DX12DescriptorHeap> _descHeap, unsigned int _descHeapIndex)
{
	mPimple->SetGraphicsRootDescriptorTable(_rootParamIndex, _descHeap, _descHeapIndex);
}

void DX12::SetResourceBarrier(boost::shared_ptr<DX12Resource> _resource, DX12Config::ResourceBarrierState _before, DX12Config::ResourceBarrierState _after)
{
	mPimple->SetResourceBarrier(_resource, _before, _after);
}

boost::shared_ptr<DX12SwapChain> DX12::CreateSwapChain(HWND _hwnd, UINT _width, UINT _height)
{
	return mPimple->CreateSwapChain(_hwnd, _width, _height);
}

void DX12::FlipSwapChain(boost::shared_ptr<DX12SwapChain> _swapchain)
{
	mPimple->FlipSwapChain(_swapchain);
}

void DX12::OpenRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain)
{
	mPimple->OpenRenderTarget(_swapchain);
}

void DX12::CloseRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain)
{
	mPimple->CloseRenderTarget(_swapchain);
}

void DX12::ClearRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain, float _r, float _g, float _b)
{
	mPimple->ClearRenderTarget(_swapchain, _r, _g, _b);
}

boost::shared_ptr<DX12Resource> DX12::CreateClearTexture(UINT64 _width, UINT64 _height, float _r, float _g, float _b, float _alpha)
{
	return mPimple->CreateClearTexture(_width, _height, _r, _g, _b,_alpha);
}

void DX12::CreateRenderTargetView(boost::shared_ptr<DX12Resource> _resource, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n)
{
	mPimple->CreateRenderTargetView(_resource, _descheap, _n);
}

void DX12::CreateShaderResourceView(boost::shared_ptr<DX12Resource> _resource, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n)
{
	mPimple->CreateShaderResourceView(_resource, _descheap, _n);
}
