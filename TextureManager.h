#pragma once

class Game;
class DX12Resource;
class DX12DescriptorHeap;

class TextureManager :public boost::noncopyable {
public:
	//コンストラクタ呼び出しをGameに制限
	TextureManager(Game* const _game);
	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="_desc">ディスクリプタヒープ</param>
	/// <param name="_num">ディスクリプタヒープ上のどこにディスクリプタを置くか</param>
	/// <param name="_textureid">テクスチャID</param>
	void LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num, unsigned int _textureid);
	/// <summary>
	/// テクスチャのアンロード(DX12Resourceの解放)
	/// </summary>
	void UnloadTexture(unsigned int _textureid);
	/// <summary>
	/// DX12Resourceを取得
	/// </summary>
	boost::shared_ptr<DX12Resource> GetDX12Resource(unsigned int _textureid);
	/// <summary>
	/// DX12DescriptorHeapとHeap上のIDを返す
	/// </summary>
	/// <returns>first:DX12DescriptorHeap,second:ID</returns>
	std::pair<boost::shared_ptr<DX12DescriptorHeap>, unsigned int> GetDX12DescriptorHeap(unsigned int _textureid);
private:
	struct Texture {
	public:
		boost::shared_ptr<DX12Resource> mResource;
		boost::shared_ptr<DX12DescriptorHeap> mDescHeap;
		unsigned int mDescID;
	};
	std::map<unsigned int, Texture> mTexturesmap;
	Game* const mGame;
};