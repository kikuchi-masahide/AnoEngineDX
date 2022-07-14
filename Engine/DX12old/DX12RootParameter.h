#pragma once

/// <summary>
/// 1つのDX12DescriptorRangeは1つのDescriptorHeapと対応し，
/// 各ビューがどのようにシェーダレジスタと対応するか指定する．
/// see:https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC145%E5%9B%9E
/// </summary>
struct DX12DescriptorRange {
public:
	/// <summary>
	/// 捕捉するディスクリプタの数
	/// </summary>
	unsigned int mNumDescriptors;
	/// <summary>
	/// 捕捉するディスクリプタの種類
	/// </summary>
	DX12Config::DescriptorRangeType mType;
	/// <summary>
	/// シェーダレジスタの何番目から占有するか
	/// (例えばmBaseShaderRegister = 2,mNumDescriptors = 4,mType = CBVならば，
	/// このディスクリプタレンジはb2からb5を占有する)
	/// </summary>
	unsigned int mBaseShaderRegister;
	/// <summary>
	/// ディスクリプタヒープの何番目から占有するか
	/// (例えばmBaseHeapRegister = 2,mNumDescriptors = 4ならば，
	/// ディスクリプタヒープの2から5番目を占有する)
	/// </summary>
	unsigned int mBaseHeapRegister;
	DX12DescriptorRange(unsigned int _num, DX12Config::DescriptorRangeType _type, unsigned int _shaderreg, unsigned int _heapreg)
		:mNumDescriptors(_num), mType(_type), mBaseShaderRegister(_shaderreg),mBaseHeapRegister(_heapreg)
	{}
};

/// <summary>
/// 複数のディスクリプタレンジをまとめる構造体
/// see:https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145
/// </summary>
struct DX12RootParameter {
public:
	DX12Config::RootParameterShaderVisibility mShaderVisibility;
	/// <summary>
	/// 持っているディスクリプタレンジのvector
	/// </summary>
	std::vector<DX12DescriptorRange> mDescRanges;
};