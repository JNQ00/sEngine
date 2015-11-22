#ifndef __SRENDERER_WIN32_H__
#define __SRENDERER_WIN32_H__

#include <vector>
#include <windows.h>

#include <Headers/sRenderer.h>

#if defined(_DEBUG)
#define ASSERT_SUCCEEDED(x, msg) do {assert(x || msg); } while(false)
#else
#define ASSERT_SUCCEEDED(x, msg) do {x; } while(false)
#endif

struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGISwapChain3;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct ID3D12CommandAllocator;

namespace sRender
{

	//-------------------------------------------------------------
	class sRenderer::sRenderer_Impl
	{
	public:
		sRenderer_Impl();
		~sRenderer_Impl();

		//-------------------------------------------------------------
		void CreateRenderContext(const sRenderContextCFG& _cfg);
		void Update();
		void Destroy();

	private:
		void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
		static const UINT FrameCount = 2;

		u32 m_frameIndex;

		ID3D12Device* m_device;
		ID3D12CommandQueue* m_commandQueue;
		IDXGISwapChain3* m_swapChain;
		ID3D12DescriptorHeap* m_rtvHeap;
		u32 m_rtvDescriptorSize;
		ID3D12Resource* m_renderTargets[FrameCount];
		ID3D12CommandAllocator* m_commandAllocator;
	};

}
#endif