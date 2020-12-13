#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"

class DX12Device;
class DX12CmdAllocator;
class DX12Resource;

class DX12CmdList final :public boost::noncopyable {
public:
	DX12CmdList();
	void Initialize(DX12Device* _device, DX12CmdAllocator* _cmdalloc);
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	void CleanUp();
	void Close();
	void Reset(DX12CmdAllocator* _cmdallocator);
	void SetPrimitive(DX12Config::PrimitiveTopologyType _prim);
	void DrawInstanced(UINT _vertnum, UINT _instnum, UINT _vdataoffset, UINT _instoffset);
	void SetViewports(UINT _widthpx, UINT _heightpx, int _topleftx, int _toplefty,
		float _maxdepth, float _mindepth);
	void SetScissorrect(LONG _top, LONG _bottom, LONG _left, LONG _right);
	void DrawIndexedInstanced(UINT _indexnumperinst, UINT _instnum, UINT _indoffset, UINT _vdataoffset, UINT _instoffset);
private:
	ComPtr<ID3D12GraphicsCommandList> mCmdList;
	static D3D12_PRIMITIVE_TOPOLOGY mPrimitiveTopologyTypeCorrespond[(unsigned char)DX12Config::PrimitiveTopologyType::size];
};