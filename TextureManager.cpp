#include "TextureManager.h"
#include "Game.h"

TextureManager::TextureManager(Game* const _game)
	:mGame(_game)
{
	//_gameの中身を一応チェック
	BOOST_ASSERT(_game != nullptr);
}

void TextureManager::LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num, unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	BOOST_ASSERT_MSG(itr == mTexturesmap.end(),"TextureID duplicating");
	Texture texture;
	texture.mDescHeap = _desc;
	texture.mDescID = _num;
	texture.mResource = mGame->mdx12.LoadTexture(_filename, _desc, _num);
	mTexturesmap.insert(std::pair<unsigned int,Texture>(_textureid,texture));
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
	return itr->second.mResource;
}

std::pair<boost::shared_ptr<DX12DescriptorHeap>, unsigned int> TextureManager::GetDX12DescriptorHeap(unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	BOOST_ASSERT_MSG(itr != mTexturesmap.end(), "unregistered TextureID");
	auto desc = itr->second.mDescHeap;
	auto id = itr->second.mDescID;
	return std::pair<boost::shared_ptr<DX12DescriptorHeap>, unsigned int>(desc, id);
}