//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"

namespace DX12 {
	class ShaderResource final:public boost::noncopyable {
	public:
		//空白のShaderResourceを作成する
		ShaderResource(ComPtr<ID3D12Device> device,DX12::ResourceHeapType heap_type,UINT64 width,UINT64 height,float r,float g,float b,float alpha,LPCWSTR debug_name);
		ShaderResource(ComPtr<ID3D12Device> device, DX12::ResourceHeapType heap_type, DXGI_FORMAT format,
			D3D12_RESOURCE_DIMENSION dimension, UINT64 width, UINT64 height, D3D12_TEXTURE_LAYOUT layout,
			D3D12_RESOURCE_FLAGS resource_flag, D3D12_HEAP_FLAGS heap_flag,
			ResourceBarrierState resource_state, LPCWSTR debug_name);
		void* Map();
		void Unmap();
		DXGI_FORMAT GetDXGIFormat() const;
		UINT64 GetWidth() const;
		UINT64 GetHeight() const;
		ID3D12Resource* GetRawPtr() const;
		D3D12_RESOURCE_DESC GetDesc() const;
	private:
		ComPtr<ID3D12Resource> resource_;
		DXGI_FORMAT format_;
		//「対象画像の」幅/高さ(リソースのwidth/heightの設定とはかみ合わないことがある)
		UINT64 width_;
		UINT64 height_;
	};
}