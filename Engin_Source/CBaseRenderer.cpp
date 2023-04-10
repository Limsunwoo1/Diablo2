#include "CBaseRenderer.h"

BaseRenderer::BaseRenderer(eComponentType type)
	: Component(type)
	, mRenderStop(false)
{
}

BaseRenderer::~BaseRenderer()
{
}

void BaseRenderer::Initalize()
{
}

void BaseRenderer::Update()
{
}

void BaseRenderer::FixedUpdate()
{
}

void BaseRenderer::Render()
{
}
