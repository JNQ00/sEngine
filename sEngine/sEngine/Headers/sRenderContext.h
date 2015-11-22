#ifndef __SRENDER_CONTEXT_H__
#define __SRENDER_CONTEXT_H__

#include <Headers/sTypes.h>
#include <Headers/sTexture.h>

namespace sRender
{

	typedef struct sRenderContextCFG
	{
		bool Debug;
		u32 Width;
		u32 Height;
		sTexFormat Format;
		u16 SampleCount;
		void* PlatformData;

		sRenderContextCFG()
		{
			Debug = true;
			Width = 800;
			Height = 600;
			Format = sTexFormat_RGBA8_Unorm;
			SampleCount = 1;
			PlatformData = 0;
		}

	}sRenderContextCFG;

}
#endif
