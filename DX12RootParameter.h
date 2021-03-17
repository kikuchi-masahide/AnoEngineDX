#pragma once

/// <summary>
/// 1つのDX12DescriptorRangeは，1つのDescriptorHeapの持つ
/// 同種の連続したディスクリプタをシェーダレジスタにバインドする
/// see:https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145
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
	DX12DescriptorRange(unsigned int _num, DX12Config::DescriptorRangeType _type, unsigned int _reg)
		:mNumDescriptors(_num), mType(_type), mBaseShaderRegister(_reg)
	{}
};

/// <summary>
/// 複数のディスクリプタレンジをまとめる構造体
/// (本当はディスクリプタレンジはディスクリプタテーブルによりまとめられ，複数のテーブルがルートパラメタによりまとめられるが，
/// ここでは簡単のため全ディスクリプタレンジを1つのテーブルにまとめ，それを1つのルートパラメタが持つようにした)
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