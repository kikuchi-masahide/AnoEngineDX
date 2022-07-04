#include "TextureManager.h"
#include "Game.h"

TextureManager::TextureManager(Game* const _game)
	:mGame(_game)
{
	//_gameの中身を一応チェック
	BOOST_ASSERT(_game != nullptr);
}

void TextureManager::LoadTexture(const wchar_t* _filename, unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	BOOST_ASSERT_MSG(itr == mTexturesmap.end(),"TextureID duplicating");
	std::wstring str(L"LoadTexture buffer for ");
	str += _filename;
	auto ret = mGame->dx12_.LoadTexture(_filename, str.c_str());
	mTexturesmap.insert(std::pair<unsigned int,boost::shared_ptr<DX12Resource>>(_textureid,ret.resource_));
	mFormat.insert(std::pair<unsigned int, unsigned char>(_textureid, ret.format_));
	return;
}

void TextureManager::UnloadTexture(unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	BOOST_ASSERT_MSG(itr != mTexturesmap.end(),"unregistered TextureID");
	mTexturesmap.erase(itr);
}

boost::shared_ptr<DX12Resource> TextureManager::GetDX12Resource(unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	BOOST_ASSERT_MSG(itr != mTexturesmap.end(), "unregistered TextureID");
	return itr->second;
}

void TextureManager::CreateSRVof(unsigned int texture_id, boost::shared_ptr<DX12DescriptorHeap> desc_heap, unsigned int heap_ind)
{
	auto resource = GetDX12Resource(texture_id);
	unsigned char format = mFormat.find(texture_id)->second;
	mGame->dx12_.CreateShaderResourceView(resource, desc_heap, heap_ind, format);
}
