#include <Headers/sRenderer.h>
#include <Tools/CommonTools.h>

#ifdef _WIN32
#include <Headers/Win32/sRenderer_win32.h>
#endif

using namespace sRender;

//-------------------------------------------------------------
sRenderer::sRenderer()
{
	m_impleme = new sRenderer_Impl();
}

//-------------------------------------------------------------
sRenderer::~sRenderer()
{
	SafeDelete(m_impleme);
}

//-------------------------------------------------------------
void sRenderer::CreateRenderContext(const sRenderContextCFG& _cfg)
{
	m_impleme->CreateRenderContext(_cfg);
}

//-------------------------------------------------------------
void sRenderer::Destroy()
{
	m_impleme->Destroy();
}