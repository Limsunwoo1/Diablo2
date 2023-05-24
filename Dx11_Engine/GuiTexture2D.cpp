#include "GuiTexture2D.h"
#include "..//Engin_Source/CTexture2D.h"

using namespace graphics;

namespace gui
{
	guiTexture2D::guiTexture2D()
		: mMaxIndexX(0)
		, mMaxIndexY(0)
	{
	}
	guiTexture2D::~guiTexture2D()
	{
	}
	void guiTexture2D::Update()
	{
		Texture2D* targetTex = dynamic_cast<Texture2D*>(GetTarget());

		if (targetTex == nullptr)
			return;

		ImGui::Image(targetTex->GetSRV().Get(), ImVec2(GetSize().x, GetSize().y));

		/*std::wstring wKey = targetTex->GetKey();
		std::wstring wPath = targetTex->GetPath();

		std::string key(wKey.begin(), wKey.end());
		std::string path(wPath.begin(), wPath.end());*/

		//ImGui::Text("key	"); ImGui::SameLine();
		//ImGui::InputText("##TexKey", (char*)key.data(), key.size(), ImGuiInputTextFlags_ReadOnly);

		//ImGui::Text("Path	"); ImGui::SameLine();
		//ImGui::InputText("##TexPath", (char*)key.data(), key.size(), ImGuiInputTextFlags_ReadOnly);

		//int width = targetTex->GetWidth();
		//int heigh = targetTex->GetHeight();

		//char wBuffer[256] = "";
		//// 정수를 문자열로 반환
		//_itoa_s(width, wBuffer, 10);
		//ImGui::Text("Width	"); ImGui::SameLine();
		//ImGui::InputText("##TexWidth", wBuffer, 256, ImGuiInputTextFlags_ReadOnly);

		//char hBuffer[256] = "";
		//// 정수를 문자열로 반환
		//_itoa_s(heigh, hBuffer, 10);
		//ImGui::Text("Heigh	"); ImGui::SameLine();
		//ImGui::InputText("##TexHeigh", hBuffer, 256, ImGuiInputTextFlags_ReadOnly);
		
	}
}
