#pragma once
#include "Engine/Core/Event/Event.h"
namespace Engine
{
	class Layer
	{
	public:
		Layer(std::string name = "Layer");
		virtual void OnAttach() {}
		virtual void OnUpdate() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

		virtual ~Layer();
	protected:
		std::string m_DebugName;
	};
}