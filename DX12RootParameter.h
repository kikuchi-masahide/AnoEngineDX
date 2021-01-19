#pragma once
struct DX12DescriptorRange {
public:
	unsigned int mNumDescriptors;
	DX12Config::DescriptorRangeType mType;
	unsigned int mBaseShaderRegister;
	DX12DescriptorRange(unsigned int _num, DX12Config::DescriptorRangeType _type, unsigned int _reg)
		:mNumDescriptors(_num), mType(_type), mBaseShaderRegister(_reg)
	{}
};

struct DX12RootParameter {
public:
	DX12Config::RootParameterShaderVisibility mShaderVisibility;
	std::vector<DX12DescriptorRange> mDescRanges;
};