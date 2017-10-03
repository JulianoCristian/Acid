#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class ManagerRender :
	public IManagerRender
{
private:
	Vector4 m_infinity;

	RendererTest *m_rendererTest;
public:
	ManagerRender();

	~ManagerRender();

	void Render(const VkCommandBuffer *commandBuffer) override;
};