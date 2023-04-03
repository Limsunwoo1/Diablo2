#pragma once
#include "CEntity.h"
#include "CLayer.h"

class Camera;
class Scene : public Entity
{
public:
	Scene(eSceneType);
	virtual ~Scene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void Destroy();

	virtual void OnEnter(){};
	virtual void OnExit(){};

	void AddGameObject(GameObject* gameObj, const eLayerType type);
	Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }
	std::vector<GameObject*> GetDontDestroyObjects();
	const std::vector<GameObject*>& GetGameObject(const eLayerType type);
	eSceneType GetScenType() { return mType; }

	Camera* GetMainCam() { return mMainCamera; }
private:
	std::vector<Layer> mLayers;
	eSceneType mType;

protected:
	class Camera* mMainCamera;
};

