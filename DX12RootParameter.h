#pragma once
struct DX12DescriptorRange {
public:
	unsigned int mNumDescriptors;
	DX12Config::DescriptorHeapType mType;
	unsigned int mBaseShaderRegister;
	DX12DescriptorRange(unsigned int _num, DX12Config::DescriptorHeapType _type, unsigned int _reg)
		:mNumDescriptors(_num), mType(_type), mBaseShaderRegister(_reg)
	{}
};

struct DX12RootParameter {
public:
	DX12Config::ShaderVisibility mShaderVisibility;
	std::vector<DX12DescriptorRange> mDescRanges;
};