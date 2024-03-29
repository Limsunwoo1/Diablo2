#include "GuiHierachy.h"
#include "GuiInspector.h"
#include "GuiEditor.h"

#include "..//Engin_Source/CApplication.h"
#include "..//Engin_Source/CScene.h"
#include "..//Engin_Source/CLayer.h"
#include "..//Engin_Source/CSceneManager.h"
#include "..//Engin_Source/CRenderer.h"
#include "..//Engin_Source/CUiBase.h"

#include "..//Engin_Source/CItemManager.h"
#include "..//Engin_Source/CTime.h"

extern gui::Editor _Editor;
extern CApplication Application;

namespace gui
{
    Hierachy::Hierachy()
        : mTreeWidget(nullptr)
        , mTime(0.0f)
    {
        SetName("Hierarchy");
        SetSize(ImVec2(1600 / 2, 900 / 2));

        mTreeWidget = new TreeWidget();
        mTreeWidget->SetName("Scenes");
        AddWidget(mTreeWidget);

        mTreeWidget->SetEvent(this, std::bind(&Hierachy::InitalizeInsepector, this, std::placeholders::_1));
        mTreeWidget->SetDummyRoot(true);

        InitalizeScene();
    }
    Hierachy::~Hierachy()
    {
        delete mTreeWidget;
        mTreeWidget = nullptr;
    }
    void Hierachy::FixedUpdate()
    {
    }
    void Hierachy::Update()
    {
        mTime += Time::GetInstance()->DeltaTime();

        if (mTime >= 2.0f)
        {
            mTime -= 2.0f;
            InitalizeScene();
        }

    }
    void Hierachy::LateUpdate()
    {
    }
    void Hierachy::InitalizeInsepector(void* data)
    {
        Renderer::InspectorGameObject = static_cast<GameObject*>(data);

        Inspector* inspector = _Editor.GetWidget<Inspector>("Inspector");
        inspector->SetTargetGameObject(Renderer::InspectorGameObject);
        inspector->InitalizeTargetGameObject();
    }
    void Hierachy::InitalizeScene()
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

                if (i == (UINT)eLayerType::UI)
                {
                    UiChilds(obj, root);
                }
            }
        }

        const std::vector<ItemBase*>& items = ItemManager::GetInstance()->GetItems();
        for (GameObject* obj : items)
        {
            AddGameObject(root, obj);
        }
    }
    void Hierachy::AddGameObject(TreeWidget::Node* parent, GameObject* gameObject)
    {
        std::wstring wName = gameObject->GetName();
        std::string Name(wName.begin(), wName.end());

        TreeWidget::Node* node = mTreeWidget->AddNode(parent, Name, gameObject);
    }
    void Hierachy::UiChilds(GameObject* obj, TreeWidget::Node* root)
    {
        UiBase* ui = dynamic_cast<UiBase*>(obj);
        if (ui == nullptr)
            return;

        const std::vector<UiBase*>& gameObjects = ui->GetChilds();

        for (UiBase* uichild : gameObjects)
        {
            AddGameObject(root, uichild);
            UiChilds(uichild, root);
        }
    }
}