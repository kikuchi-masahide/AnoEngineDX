#include "DX12CmdList.h"
#include "DX12Device.h"
#include "DX12CmdAllocator.h"
#include "DX12Resource.h"
#include "DX12DescriptorHeap.h"
#include "DX12Pimple.h"

DX12CmdList::DX12CmdList()
	:mCmdList()
{}

void DX12CmdList::Initialize(DX12Device* _device, DX12CmdAllocator* _cmdalloc)
{
	if (FAILED(
		_device->GetDevice()->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdalloc->GetCmdAllocator().Get(), nullptr,
			IID_PPV_ARGS(mCmdList.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12GraphicsCommandList Initialization failed");
		throw 0;
	}
}

ComPtr<ID3D12GraphicsCommandList> DX12CmdList::GetCmdList()
{
	return mCmdList;
}

void DX12CmdList::CleanUp() {
	mCmdList.Reset();
}

void DX12CmdList::Close()
{
	mCmdList->Close();
}

void DX12CmdList::Reset(DX12CmdAllocator* _cmdallocator)
{
	mCmdList->Reset(_cmdallocator->GetCmdAllocator().Get(), nullptr);
}

void DX12CmdList::SetPrimitive(DX12Config::PrimitiveTopologyType _prim)
{
	mCmdList->IASetPrimitiveTopology(mPrimitiveTopologyTypeCorrespond[(unsigned char)_prim]);
}

void DX12CmdList::DrawInstanced(UINT _vertnum, UINT _instnum, UINT _vdataoffset, UINT _instoffset)
{
	mCmdList->DrawInstanced(_vertnum, _instnum, _vdataoffset, _instoffset);
}

void DX12CmdList::SetViewports(UINT _widthpx, UINT _heightpx, int _topleftx, int _toplefty, float _maxdepth, float _mindepth)
{
	D3D12_VIEWPORT viewport = {};
	viewport.Width = _widthpx;
	viewport.Height = _heightpx;
	viewport.TopLeftX = _topleftx;
	viewport.TopLeftY = _toplefty;
	viewport.MaxDepth = _maxdepth;
	viewport.MinDepth = _mindepth;
	mCmdList->RSSetViewports(1, &viewport);
}

void DX12CmdList::SetScissorrect(LONG _top, LONG _bottom, LONG _left, LONG _right)
{
	D3D12_RECT scissorrect = {};
	scissorrect.top = _top;
	scissorrect.bottom = _bottom;
	scissorrect.left = _left;
	scissorrect.right = _right;
	mCmdList->RSSetScissorRects(1, &scissorrect);
}

void DX12CmdList::DrawIndexedInstanced(UINT _indexnumperinst, UINT _instnum, UINT _indoffset, UINT _vdataoffset, UINT _instoffset)
{
	mCmdList->DrawIndexedInstanced(_indexnumperinst, _instnum, _indoffset, _vdataoffset, _instoffset);
}

D3D12_PRIMITIVE_TOPOLOGY DX12CmdList::mPrimitiveTopologyTypeCorrespond[(unsigned char)DX12Config::PrimitiveTopologyType::size] = {
	D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopologyTypeCorrespond[(unsigned char)DX12Config::PrimitiveTopologyType::size] = {
	D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

void DX12Pimple::SetPrimitiveTopology(DX12Config::PrimitiveTopologyType _prim)
{
	mCmdList->IASetPrimitiveTopology(PrimitiveTopologyTypeCorrespond[(unsigned char)_prim]);
}

void DX12Pimple::DrawIndexedInstanced(UINT _indexnumperinst, UINT _instnum, UINT _indoffset, UINT _vdataoffset, UINT _instoffset)
{
	mCmdList->DrawIndexedInstanced(_indexnumperinst, _instnum, _indoffset, _vdataoffset, _instoffset);
}

void DX12Pimple::SetViewports(UINT _widthpx, UINT _heightpx, int _topleftx, int _toplefty,
	float _maxdepth, float _mindepth)
{
	D3D12_VIEWPORT viewport = {};
	viewport.Width = _widthpx;
	viewport.Height = _heightpx;
	viewport.TopLeftX = _topleftx;
	viewport.TopLeftY = _toplefty;
	viewport.MaxDepth = _maxdepth;
	viewport.MinDepth = _mindepth;
	mCmdList->RSSetViewports(1, &viewport);
}

void DX12Pimple::SetScissorrect(float _top, float _bottom, float _left, float _right)
{
	D3D12_RECT scissorrect = {};
	scissorrect.top = _top;
	scissorrect.bottom = _bottom;
	scissorrect.left = _left;
	scissorrect.right = _right;
	mCmdList->RSSetScissorRects(1, &scissorrect);
}

void DX12Pimple::SetGraphicsRootDescriptorTable(unsigned int _rootParamIndex, boost::shared_ptr<DX12DescriptorHeap> _descHeap, unsigned int _descHeapIndex)
{
	mCmdList->SetGraphicsRootDescriptorTable(_rootParamIndex, _descHeap->GetGPUDescriptorHandle(_descHeapIndex));
}
