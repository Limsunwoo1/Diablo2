#pragma once
#include "CCollider2D.h"
class AttackCollider : public Collider2D
{
public:
	AttackCollider();
	virtual ~AttackCollider();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnCollisionEnter(Collider2D* collider);

	template<typename T>
	T* CastType(eLayerType type, GameObject* obj)
	{
		if (type == eLayerType::Player)
		{
			return dynamic_cast<T*>(obj);
		}
		else if (type == eLayerType::Monster)
		{
			return dynamic_cast<T*>(obj);
		}
	}

private:


};