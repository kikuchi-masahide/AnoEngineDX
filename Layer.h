#pragma once

class Scene;
class DX12Resource;
class DX12DescriptorHeap;
class Game;
template<class T>
class LayerHandle;

class Layer {
public:
	Layer(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap);
	virtual ~Layer();
	//Output�R���|�[�l���g�EScene::UniqueOutput�̌��z�~���ŌĂяo��
	//�y���|���S���̃o�b�N�o�b�t�@�ւ̍ŏI�I�ȕ`����s���D
	virtual void Draw() = 0;
	//��L���W���{���ɕύX�����̂�Draw���s�O
	void SetRect(Rect2 _rect);
	Rect2 GetRect();
	//z���W���{���ɕύX�����̂�Draw���s�O
	void SetZ(double _z);
	double GetZ();
	//z���W�̕ύX����������
	bool HasZChanged();
	//�e�V�[��
	Scene& mScene;
	Game& GetGame() const;
	//Scene����̂݌Ăяo����CRect�Cz�̕ύX�t���b�V���p�̊֐�
	void FlushZRectChange(Layer* _layer);
	bool GetDeleteFlag() const
	{
		return mDeleteFlag;
	}
	void SetDeleteFlag()
	{
		mDeleteFlag = true;
	}
	unsigned int GetSwapchainID();
protected:
	template<class T>
	LayerHandle<T> This()
	{
		return LayerHandle<T>((T*)this, mHandles);
	}
private:
	//�������w���n���h���̏W���̃|�C���^(void*���g���͉̂��Ƃ�������߂���)
	boost::shared_ptr<std::set<void*>> mHandles;
	//&�ɂ��|�C���^�擾���֎~
	Layer* operator&() const noexcept;
	//Draw���s�O�̕ύX�̃X�g�b�N�Ƃ��̃t���O
	bool mWasZChanged;
	bool mWasRectChanged;
	double z;
	double mZAfter;
	//�N���C�A���g��̐�L�̈�
	Rect2 mRect;
	Rect2 mRectAfter;
	unsigned int mSwapchainID;
	bool mDeleteFlag;
};