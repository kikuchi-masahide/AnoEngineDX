#pragma once

class Game;
class DX12Resource;
class DX12DescriptorHeap;

class TextureManager :public boost::noncopyable {
public:
	TextureManager(Game* const _game);
private:
	struct Texture {
	public:
		boost::shared_ptr<DX12Resource> mResource;
		boost::shared_ptr<DX12DescriptorHeap> mDescHeap;
		unsigned int mDescID;
	};
	std::map<unsigned int, Texture> mTexturesmap;
	unsigned int LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num);
	void UnloadTexture(unsigned int _textureid);
	boost::shared_ptr<DX12Resource> GetDX12Resource(unsigned int _textureid);
	std::pair<boost::shared_ptr<DX12DescriptorHeap>, unsigned int> GetDX12DescriptorHeap(unsigned int _textureid);
	Game* const mGame;
};