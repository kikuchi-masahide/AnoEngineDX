#pragma once
#include "Component.h"
#include "ComponentHandle.h"
#include "LayerHandle.h"
#include "Game.h"

class Layer2D;

class Layer2DComponent :public Component {
public:
	Layer2DComponent(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, LayerHandle<Layer2D> _layer, unsigned int _texture, double _z, Rect2 _rect);
	virtual void Update();
	~Layer2DComponent();
	//Layer2D����Ăяo�����
	virtual void Draw();
	struct VertexLayout {
	public:
		Vector3 mPos;
		Vector2 mUV;
	};
	//Layer����𒆐S�Ƃ���1px�P�ʂ̍��W
	//1 3
	//0 2
	Vector3 mVerts[4];
	//UV���W
	Vector2 mUV[4];
	double z;
protected:
	LayerHandle<Layer2D> mLayer;
	unsigned int mTextureID;
};