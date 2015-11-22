#ifndef __SRENDERER_H__
#define __SRENDERER_H__

#include <Headers/sTypes.h>
#include <Headers/sRenderContext.h>

namespace sRender
{

	class sRenderer
	{
	public:
		sRenderer();
		virtual ~sRenderer();

		void CreateRenderContext(const sRenderContextCFG& _cfg);
		void Update();
		void Destroy();
	private:
		class sRenderer_Impl;
		sRenderer_Impl* m_impleme;
	};

}
#endif
