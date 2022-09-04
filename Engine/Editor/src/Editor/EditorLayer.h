#pragma once
#include "Engine/Core/Layer/Layer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ThemeEditorPanel.h"
#include "Panels/AnimatorEditorPanel.h"
#include "Engine/Renderer/Camera/Camera.h"
namespace Engine
{
	class EditorLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnUpdate() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
	private:
		void DrawPanels();
		Ref<Camera> myEditorCamera;
		Ref<Scene> myScene;
		Ref<AnimatorEditorPanel> myAnimatorEditorPanel;
		Ref<ThemeEditorPanel> myThemeEditorPanel;
		Ref<SceneHierarchyPanel> mySceneHierarchyPanel;
		Ref<ContentBrowserPanel> myContentBrowserPanel;
	};
}
