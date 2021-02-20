#include "TextureManager.h"
#include "Game.h"

TextureManager::TextureManager(Game* const _game)
	:mGame(_game)
{
	//_gameの中身を一応チェック
	assert(_game != nullptr);
}

unsigned int TextureManager::LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num)
{
	//次に使うID
	static unsigned int nextid = 0;
	Texture texture;
	texture.mDescHeap = _desc;
	texture.mDescID = _num;
	texture.mResource = mGame->mdx12.LoadTexture(_filename, _desc, _num);
	mTexturesmap.insert(std::pair<unsigned int,Texture>(nextid,texture));
	return nextid++;
}

void TextureManager::UnloadTexture(unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	assert(itr != mTexturesmap.end());
	mTexturesmap.erase(itr);
}

boost::shared_ptr<DX12Resource> TextureManager::GetDX12Resource(unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	assert(itr != mTexturesmap.end());
	return itr->second.mResource;
}

std::pair<boost::shared_ptr<DX12DescriptorHeap>, unsigned int> TextureManager::GetDX12DescriptorHeap(unsigned int _textureid)
{
	auto itr = mTexturesmap.find(_textureid);
	assert(itr != mTexturesmap.end());
	auto desc = itr->second.mDescHeap;
	auto id = itr->second.mDescID;
	return std::pair<boost::shared_ptr<DX12DescriptorHeap>, unsigned int>(desc, id);
}