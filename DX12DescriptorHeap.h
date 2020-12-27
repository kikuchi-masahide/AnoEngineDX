#pragma once

#include "DX12CommonInclude.h"

#include "DX12ConfigEnums.h"

class DX12Device;
class DX12Resource;

class DX12DescriptorHeap final :public boost::noncopyable {
public:
	//コンストラクタ
	DX12DescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num, DX12Device* _device);
	~DX12DescriptorHeap();
	/// <summary>
	/// _n番目のCPUハンドル取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(unsigned int _n);
private:
	//所有ディスクリプタの数
	int mNum;
	DX12Config::DescriptorHeapType mType;
	DX12Config::ShaderVisibility mShaderVisibility;
	ComPtr<ID3D12DescriptorHeap> mDescHeap;
	//1ディスクリプタ当たりのディスクリプタハンドルハンドル
	SIZE_T mDescriptorHandleSize;
	//ディスクリプタヒープ初期化で用いる構造体(共通する設定はこれに設定しておく)
	static D3D12_DESCRIPTOR_HEAP_DESC mHeapDescStr;
	//DX12Config::DescriptorHeapTypeの各定数が初期化時どの定数に対応するか
	static D3D12_DESCRIPTOR_HEAP_TYPE mDescriptorHeapTypeCorrespond[(unsigned char)DX12Config::DescriptorHeapType::size];
	//DX12Config::ShaderVisibilityの各定数が初期化時どの定数に対応するか
	static D3D12_DESCRIPTOR_HEAP_FLAGS mShaderVisibilityCorrespond[(unsigned char)DX12Config::ShaderVisibility::size];
};