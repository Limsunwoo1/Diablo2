#pragma once
#include "CEngine.h"

using namespace std;

union ColliderID
{
	struct
	{
		UINT32 left;
		UINT32 right;
	};
	UINT64 id;
};

class CollisionManager
{
	SINGLE(CollisionManager);
public:
	void Initalize();
	void Update();
	void FixedUpdate();
	void Render();

	void CollisionlayerCheck(eLayerType left, eLayerType right, bool enable = true);
	void LayerCollision(class Scene* scene, eLayerType left, eLayerType right);
	void ColliderCollision(class Collider2D* left, Collider2D* right);
	bool Intersect(Collider2D* left, Collider2D* right);
	bool Rect_VS_Rect(Collider2D* left, Collider2D* right);
	bool Circle_VS_Circle(Collider2D* left, Collider2D* right);

	bool ButtonCollisionCheck(const Vector2& mousePos, Collider2D* button);

private:
	bitset<(UINT)eLayerType::End> mLayerCollisionMatrix[(UINT)eLayerType::End];
	map<UINT64, bool> mCollisionMap;
};
