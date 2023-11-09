#include "SoftwareRenderer.h"
#include "WinGDISurface.h"
#include "SpriteInstance.h"

/*****************************************************************************************/
SoftwareRenderer* SoftwareRenderer::s_currentInstance = NULL;

/*****************************************************************************************/
SoftwareRenderer::SoftwareRenderer(HWND window)
{
	m_surface = new WinGDISurface(window);
	m_surface->SetBlendMode(COMPARE_DEPTH);
}

/*****************************************************************************************/
void SoftwareRenderer::Create(HWND window)
{
	s_currentInstance = new SoftwareRenderer(window);
}

/*****************************************************************************************/
void SoftwareRenderer::Destroy()
{
	delete s_currentInstance;
}

/*****************************************************************************************/
SoftwareRenderer::~SoftwareRenderer()
{
	delete m_surface;
}

/*****************************************************************************************/
void SoftwareRenderer::Reset()
{
	m_surface->Clear();
}

/*****************************************************************************************/
void SoftwareRenderer::Present()
{
	m_surface->BlitToWindow();
}

/*****************************************************************************************/
void SoftwareRenderer::Render(SpriteInstance* spriteInstance)
{
	m_surface->AddSprite(spriteInstance);
}

/*****************************************************************************************/
SoftwareRenderer* SoftwareRenderer::GetCurrentInstance()
{
	return s_currentInstance;
}

/*****************************************************************************************/
BlendMode SoftwareRenderer::GetBlendMode() const
{
	return m_surface->GetBlendMode();
}

/*****************************************************************************************/
void SoftwareRenderer::SetBlendMode(BlendMode mode)
{
	m_surface->SetBlendMode(mode);
}
