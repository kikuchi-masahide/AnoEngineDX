#pragma once
#include "Component.h"
#include "ComponentHandle.h"
#include "LayerHandle.h"
#include "Game.h"

class Layer2D;

/// <summary>
/// Layer2D�Ɍ��ѕt����ꂽ�o�̓R���|�[�l���g
/// </summary>
class Layer2DComponent :public Component {
public:
	/// <param name="_layer">����Layer2D</param>
	/// <param name="_texture">�`�悷��e�N�X�`��</param>
	/// <param name="_z">z���W �傫���قǉ���</param>
	/// <param name="_rect">���C���[��̕`��͈�(1px�P��)</param>
	Layer2DComponent(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, LayerHandle<Layer2D> _layer, unsigned int _texture, double _z, Rect2 _rect);
	virtual void Update();
	~Layer2DComponent();
	/// <summary>
	/// Layer2D�̉�ʏo�͎��C�����_�[�^�[�Q�b�g���y���|���S���Ɏw�肵����ԂŌĂяo�����D
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// Layer����𒆐S�Ƃ���1px�P�ʂ̍��W
	/// 1 3
	/// 0 2
	/// </summary>
	Vector3 mVerts[4];
	/// <summary>
	/// UV���W
	/// 1 3
	/// 0 2
	/// </summary>
	Vector2 mUV[4];
	double z;
protected:
	LayerHandle<Layer2D> mLayer;
	unsigned int mTextureID;
};