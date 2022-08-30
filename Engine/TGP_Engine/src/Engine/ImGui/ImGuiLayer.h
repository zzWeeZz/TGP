#pragma once
#include "Engine/Core/Layer/Layer.h"
namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnUpdate() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
	};
}