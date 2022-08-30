#pragma once
#include "Engine/DX11/DX11.h"
namespace Engine
{
	struct PassSpecs
	{

	};
	class RenderPass
	{
	public:
		RenderPass(PassSpecs& specs);
		void Begin();
		void End();
		static Ref<RenderPass> Create(PassSpecs& specs);
	private:
		PassSpecs m_Data;
	};
}