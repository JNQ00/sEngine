#include "sRenderer_win32.h"
#include "Tools/Log.hpp"
#include "Tools/CommonTools.h"

#include <assert.h>
#include <D3D12.h>
#include "d3dx12.h"
#include "DXGI1_4.h"

#define WARP_DEVICE false

using namespace sRender;

//-------------------------------------------------------------
sRenderer::sRenderer_Impl::sRenderer_Impl()
{

}

//------------------------------------------------------------- 
sRenderer::sRenderer_Impl::~sRenderer_Impl()
{
}

//-------------------------------------------------------------
void sRenderer::sRenderer_Impl::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;
	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter1* pAdapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
		{
			// No more adapters to enumerate.
			break;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			*ppAdapter = pAdapter;
			return;
		}
		pAdapter->Release();
	}
}

//-------------------------------------------------------------
void sRenderer::sRenderer_Impl::Update()
{

}

//-------------------------------------------------------------
void sRenderer::sRenderer_Impl::Destroy()
{
	ReleaseCOM(m_device);
	ReleaseCOM(m_commandQueue);
	ReleaseCOM(m_swapChain);
	ReleaseCOM(m_rtvHeap);
	ReleaseCOM(m_commandAllocator);
	for (u32 i = 0; i < FrameCount; ++i)
	{
		ReleaseCOM(m_renderTargets[i]);
	}
}

//-------------------------------------------------------------
void sRenderer::sRenderer_Impl::CreateRenderContext(const sRenderContextCFG& _cfg)
{
	// Enable the D3D12 debug layer.
	if (_cfg.Debug)
	{
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
	
	IDXGIFactory4* factory;
	ASSERT_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), "Could not create DXGI factory!");

	if (WARP_DEVICE)
	{
		IDXGIAdapter* warpAdapter;
		ASSERT_SUCCEEDED(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)), "Could not enumerate warp adapters!");
		
		ASSERT_SUCCEEDED(D3D12CreateDevice(
			warpAdapter,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
			), "Could not create DX12 device!");
	}
	else
	{
		IDXGIAdapter1* hardwareAdapter;
		GetHardwareAdapter(factory, &hardwareAdapter);

		ASSERT_SUCCEEDED(D3D12CreateDevice(
			hardwareAdapter,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
			), "Could not create DX12 device!");
	}
	
	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ASSERT_SUCCEEDED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), "Could not create a command queue!");

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.BufferDesc.Width = _cfg.Width;
	swapChainDesc.BufferDesc.Height = _cfg.Height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = (HWND)_cfg.PlatformData;
	swapChainDesc.SampleDesc.Count = _cfg.SampleCount;
	swapChainDesc.Windowed = TRUE;

	IDXGISwapChain* swapChain;
	ASSERT_SUCCEEDED(factory->CreateSwapChain(
		m_commandQueue,        // Swap chain needs the queue so that it can force a flush on it.
		&swapChainDesc,
		&swapChain
		), "Could not create swap chain!");

	m_swapChain = (IDXGISwapChain3*)swapChain;

/*
// This sample does not support fullscreen transitions.
ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
*/
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ASSERT_SUCCEEDED(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)), "Could not create descriptor heap!");

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			ASSERT_SUCCEEDED(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])), "Could not getbuffer from swap chain!");
			m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}

	ASSERT_SUCCEEDED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)), "Could not create command allocator!");

}