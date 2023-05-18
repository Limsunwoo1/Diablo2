#include "GuiHierachy.h"
#include "GuiInspector.h"
#include "GuiEditor.h"

#include "..//Engin_Source/CApplication.h"
#include "..//Engin_Source/CScene.h"
#include "..//Engin_Source/CLayer.h"
#include "..//Engin_Source/CSceneManager.h"
#include "..//Engin_Source/CRenderer.h"

extern gui::Editor _Editor;
extern CApplication Application;

namespace gui
{
    Hierarchy::Hierarchy()
        : mTreeWidget(nullptr)
    {
        SetName("Hierarchy");
        SetSize(ImVec2(1600 / 2, 900 / 2));

        mTreeWidget = new TreeWidget();
        mTreeWidget->SetName("Scenes");
        AddWidget(mTreeWidget);

        mTreeWidget->SetEvent(this, std::bind(&Hierarchy::InitalizeInsepector, this, std::placeholders::_1));
        mTreeWidget->SetDummyRoot(true);

        InitalizeScene();
    }
    Hierarchy::~Hierarchy()
    {
        delete mTreeWidget;
        mTreeWidget = nullptr;
    }
    void Hierarchy::FixedUpdate()
    {
    }
    void Hierarchy::Update()
    {
    }
    void Hierarchy::LateUpdate()
    {
    }
    void Hierarchy::InitalizeInsepector(void* data)
    {
        Renderer::InspectorGameObject = static_cast<GameObject*>(data);

        Inspector* inspector = _Editor.GetWidget<Inspector>("Inspector");
        inspector->SetTargetGameObject(Renderer::InspectorGameObject);
        inspector->InitalizeTargetGameObject();
    }
    void Hierarchy::InitalizeScene()
    {
        mTreeWidget->Clear();

        Scene* scene = SceneManager::GetInstance()->GetActiveScene();
        if (scene == nullptr)
            return;

        std::wstring wNameScene = scene->GetName();
        std::string sceneName(wNameScene.begin(), wNameScene.end());

        TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);
        
        for (size_t i = 0; i < (UINT)eLayerType::End; i++) 
        {
            Layer& layer = scene->GetLayer((eLayerType)i);
            const std::vector<GameObject*>& gameObjects = layer.GetGameObjects();

            for (GameObject* obj : gameObjects)
            {
                AddGameObject(root, obj);
            }
        }

    }
    void Hierarchy::AddGameObject(TreeWidget::Node* parent, GameObject* gameObject)
    {
        std::wstring wName = gameObject->GetName();
        std::string Name(wName.begin(), wName.end());

        TreeWidget::Node* node = mTreeWidget->AddNode(parent, Name, gameObject);
    }
}