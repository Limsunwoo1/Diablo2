#include "GuiProject.h"

#include "..//Engin_Source/CTexture2D.h"
#include "..//Engin_Source/CMaterial.h"
#include "..//Engin_Source/CMesh.h"
#include "..//Engin_Source/CShader.h"

namespace gui
{
    Project::Project()
        : mTreeWidget(nullptr)
    {
        SetName("Project");

        UINT width = 1600;
        UINT height = 900;

        Vector2 size(width, height);

        SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

        mTreeWidget = new GuiTreeWidget();
        mTreeWidget->SetName("Resources");
        AddWidget(mTreeWidget);

        mTreeWidget->SetDummyRoot(true);
        ResetContent();
    }
    Project::~Project()
    {
        delete mTreeWidget;
        mTreeWidget = nullptr;
    }
    void Project::FixedUpdate()
    {
        Widget::FixedUpdate();

        // 리소스가 바뀐다면 리소스 목록 초기화
    }
    void Project::Update()
    {
        Widget::Update();
    }
    void Project::LateUpdate()
    {
    }
    void Project::ResetContent()
    {
        mTreeWidget->Clear();

        GuiTreeWidget::Node* pRootNode = mTreeWidget->AddNode(nullptr, "Resources", 0, true);

        AddResources<Mesh>(pRootNode, "Mesh");
        AddResources<graphics::Texture2D>(pRootNode, "Texture");
        AddResources<Material>(pRootNode, "Material");
        AddResources<Shader>(pRootNode, "Shader");
    }
    void Project::toInspector()
    {
    }
}