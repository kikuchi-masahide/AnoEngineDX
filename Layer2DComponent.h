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
	/// <summary>
	/// Layer2D���z���W�DmVerts��z���W�͕`�揇���Ɋ֗^���Ȃ����Ƃɒ���
	/// </summary>
	double z;
	/// <summary>
	/// �`��̈�̕��s�ړ�
	/// </summary>
	void TranslationRect(Vector3 _vect);
	/// <summary>
	/// �`��̈���C�̈�̒��S�𒆐S�Ƃ��Ċe�������Ɋg��
	/// </summary>
	/// <param name="_x">x�������̊g�嗦</param>
	/// <param name="_y">y�������̊g�嗦</param>
	/// <param name="_z">z�������̊g�嗦</param>
	void ExpandRect(double _x, double _y, double _z);
	/// <summary>
	/// �`��̈���C_center�𒆐S�Ƃ��Ċe�������Ɋg��
	/// </summary>
	/// <param name="_x">x�������̊g�嗦</param>
	/// <param name="_y">y�������̊g�嗦</param>
	/// <param name="_z">z�������̊g�嗦</param>
	/// <param name="_center">�g�咆�S</param>
	void ExpandRect(double _x, double _y, double _z, Vector3 _center);
	/// <summary>
	/// �`��̈���C���̒��S�𒆐S�Ƃ���_vect������_deg��]
	/// </summary>
	void RotateRect(Vector3 _vect, double _deg);
	/// <summary>
	/// �`��̈���C_center�𒆐S�Ƃ���_vect������_deg��]
	/// </summary>
	void RotateRect(Vector3 _vect, double _deg, Vector3 _center);
	/// <summary>
	/// �`��̈�ɂ��̍s���K�p
	/// </summary>
	void ApplyMatrix(Matrix4x4 _mat);
protected:
	LayerHandle<Layer2D> mLayer;
	unsigned int mTextureID;
};