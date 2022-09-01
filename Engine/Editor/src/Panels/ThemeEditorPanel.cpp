#include "ThemeEditorPanel.h"
#include "imgui.h"
#include "yaml-cpp/yaml.h"
#include <fstream>

void Engine::ThemeEditorPanel::StartUp()
{
	LoadTheme();
}

void Engine::ThemeEditorPanel::OnImGuiRender()
{
	ImGui::Begin("Theme Editor");
	ImGui::Text("Color: ");
	ImGui::SameLine();
	static float col[3] = { 0 };
	if (ImGui::ColorEdit3("##", col))
	{
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(col[0], col[1], col[2], 1.f);
	}
	if (ImGui::Button("Apply"))
	{
		memcpy(&mySavedColor, col, sizeof(float) * 3);
		SaveTheme();
	}
	ImGui::SameLine();
	if (ImGui::Button("Revert"))
	{
		memcpy(col, &mySavedColor, sizeof(float) * 3);
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(col[0], col[1], col[2], 1.f);
	}
	ImGui::End();
}

void Engine::ThemeEditorPanel::SaveTheme()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "ColorX" << YAML::Value << mySavedColor.x;
	out << YAML::Key << "ColorY" << YAML::Value << mySavedColor.y;
	out << YAML::Key << "ColorZ" << YAML::Value << mySavedColor.z;
	out << YAML::EndMap;
	std::ofstream fout("Assets/Themes/Theme.yaml");
	fout << out.c_str();
}

void Engine::ThemeEditorPanel::LoadTheme()
{
	std::ifstream fin("Assets/Themes/Theme.yaml");
	std::stringstream buffer;
	buffer << fin.rdbuf();
	YAML::Node node = YAML::Load(buffer.str());
	mySavedColor.x = node["ColorX"].as<float>();
	mySavedColor.y = node["ColorY"].as<float>();
	mySavedColor.z = node["ColorZ"].as<float>();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(mySavedColor.x, mySavedColor.y, mySavedColor.z, 1.f);
}
