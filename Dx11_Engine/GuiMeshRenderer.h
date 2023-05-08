#pragma once
#include "GuiComponent.h"

#include "..//Engin_Source/CMaterial.h"
#include "..//Engin_Source/CMesh.h"

namespace gui
{
	class guiMeshRenderer : public gui::Component
	{
	public:
		guiMeshRenderer();
		virtual ~guiMeshRenderer();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		std::shared_ptr<Material> mMaterial;
		std::shared_ptr<Mesh> mMesh;
	};
}

