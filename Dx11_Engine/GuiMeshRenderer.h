#pragma once
#include "GuiComponent.h"

#include "..//Engin_Source/CMaterial.h"
#include "..//Engin_Source/CMesh.h"

namespace gui
{
	class MeshRenderer : public gui::Component
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		std::weak_ptr<Material> mMaterial;
		std::weak_ptr<Mesh> mMesh;
	};
}

