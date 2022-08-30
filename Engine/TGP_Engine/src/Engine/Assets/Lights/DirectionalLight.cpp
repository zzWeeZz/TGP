#include "DirectionalLight.h"

Engine::Ref<Engine::DirectionalLight> Engine::DirectionalLight::Create()
{
	return CreateRef<DirectionalLight>();
}
