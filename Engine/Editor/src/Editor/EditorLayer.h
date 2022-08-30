#pragma once
#include "Engine/Core/Layer/Layer.h"
#include "Panels/SceneHierarchyPanel.h"
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
		Ref<Camera> myEditorCamera;
		Ref<Scene> myScene;
		Ref<SceneHierarchyPanel> mySceneHierarchyPanel;
	};
}
