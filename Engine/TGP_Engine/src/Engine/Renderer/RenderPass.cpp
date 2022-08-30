#include "RenderPass.h"
namespace Engine
{
	RenderPass::RenderPass(PassSpecs& specs)
	{
		memcpy(&m_Data, &specs, sizeof(PassSpecs));
		

	}
	Ref<RenderPass> RenderPass::Create(PassSpecs& specs)
	{
		return CreateRef<RenderPass>(specs);
	}
}