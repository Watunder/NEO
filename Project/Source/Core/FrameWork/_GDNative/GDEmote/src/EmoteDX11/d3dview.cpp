//  ◇UTF-8

#include <windows.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <wrl.h>
#include <dxgidebug.h>
using Microsoft::WRL::ComPtr;


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxguid.lib")

#define USE_SWAP_CHAIN 0

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "d3dview.h"

//------------------------------------------------

static HWND shWnd;
static int sWidth;
static int sHeight;

static IDXGISwapChain* spSwapChain;
static ID3D11Device* spDevice;
static ID3D11DeviceContext* spDeviceContext;
static D3D_FEATURE_LEVEL sFeatureLevel;

static ID3D11RenderTargetView* spRenderTargetView;
static ID3D11Texture2D* spDepthStencilTexture;
static ID3D11DepthStencilView* spDepthStencilView;

//------------------------------------------------
IDXGISwapChain*
D3DView_SwapChain(void)
{
	return spSwapChain;
}

ID3D11Device*
D3DView_Device(void)
{
	return spDevice;
}

ID3D11DeviceContext*
D3DView_Context(void)
{
	return spDeviceContext;
}

ID3D11RenderTargetView*
D3DView_RenderTextureView(void)
{
	return spRenderTargetView;
}

ID3D11DepthStencilView*
D3DView_DepthStencilView(void)
{
	return spDepthStencilView;
}

//------------------------------------------------
int
D3DView_CreateDevice(HWND hwnd)
{
	shWnd = hwnd;

	RECT r;
	GetClientRect(shWnd, &r);
	sWidth = r.right - r.left;
	sHeight = r.bottom - r.top;

	D3D_DRIVER_TYPE type = D3D_DRIVER_TYPE_HARDWARE;
	UINT flags = 0;
	const D3D_FEATURE_LEVEL fl[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	D3D_FEATURE_LEVEL featureLevel;

	DWORD deviceFlag = D3D11_CREATE_DEVICE_DEBUG
		| D3D11_CREATE_DEVICE_BGRA_SUPPORT
		;
#if USE_SWAP_CHAIN
	DXGI_SWAP_CHAIN_DESC scd = {0};
	scd.BufferDesc.Width = sWidth;
	scd.BufferDesc.Height = sHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = shWnd;
	scd.Windowed = TRUE;
	scd.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
	  NULL, /* pAdapter */
	  D3D_DRIVER_TYPE_HARDWARE,  /* DriverType */
	  NULL, /* Software */
	  deviceFlag, /* Flags */
	  fl, /* pFeatureLevels */
	  sizeof(fl) / sizeof(fl[0]), /* FeatureLevels */
	  D3D11_SDK_VERSION, /* SDKVersion */
	  &scd, /* pSwapChainDesc */
	  &spSwapChain, /* ppSwapChain */
	  &spDevice, /* ppDevice */
	  &featureLevel, /* pFeatureLevel */
	  &spDeviceContext /* ppImmediateContext */
	);
#else
	HRESULT hr = D3D11CreateDevice(
		NULL, /* pAdapter */
		D3D_DRIVER_TYPE_HARDWARE,  /* DriverType */
		NULL, /* Software */
		deviceFlag, /* Flags */
		fl, /* pFeatureLevels */
		sizeof(fl) / sizeof(fl[0]), /* FeatureLevels */
		D3D11_SDK_VERSION, /* SDKVersion */
		&spDevice, /* ppDevice */
		&featureLevel, /* pFeatureLevel */
		&spDeviceContext /* ppImmediateContext */
	);
#endif

	if (FAILED(hr)) {
		return -1;
	}

	return 0;
}

//------------------------------------------------
int
D3DView_CreateBuffer(void)
{
#if USE_SWAP_CHAIN
	HRESULT hr;

	// バックバッファを作ってレンダーターゲットビューに割り当てる
	ID3D11Texture2D* pBackBuffer;
	spSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer) );
	hr = spDevice->CreateRenderTargetView(pBackBuffer, NULL, &spRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) {
		return -1;
	}

	// デプスバッファの作成
	D3D11_TEXTURE2D_DESC depthDesc = {0};
	depthDesc.Width = sWidth;
	depthDesc.Height = sHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	hr = spDevice->CreateTexture2D( &depthDesc, NULL, &spDepthStencilTexture );
	if (FAILED(hr)) {
		return -1;
	}

	// デプスバッファをレンダーターゲットに割り当てる
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = spDevice->CreateDepthStencilView(spDepthStencilTexture, &dsvDesc, &spDepthStencilView);
	if (FAILED(hr)) {
		return -1;
	}
	spDeviceContext->OMSetRenderTargets(1, &spRenderTargetView, spDepthStencilView);

	// ビューポートの初期化
	D3D11_VIEWPORT vp = {0};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)sWidth;
	vp.Height = (float)sHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	spDeviceContext->RSSetViewports(1, &vp);
#endif
	return 0;
}

//------------------------------------------------
int
D3DView_DestroyBuffer(void)
{
	SAFE_RELEASE(spDepthStencilView);
	SAFE_RELEASE(spDepthStencilTexture);
	SAFE_RELEASE(spRenderTargetView);

	return 0;
}

//------------------------------------------------
int
D3DView_DestroyDevice(void)
{
	SAFE_RELEASE(spSwapChain);
	SAFE_RELEASE(spDeviceContext);

	SAFE_RELEASE(spDevice);

	return 0;
}

//------------------------------------------------
int
D3DView_BeforeRender(void)
{
	HRESULT hr;

	// bs : opaque
	D3D11_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	D3D11_RENDER_TARGET_BLEND_DESC &bd0 = blendDesc.RenderTarget[0];
	bd0.BlendOp = D3D11_BLEND_OP_ADD;
	bd0.SrcBlend = D3D11_BLEND_ONE;
	bd0.DestBlend = D3D11_BLEND_ZERO;
	bd0.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd0.SrcBlendAlpha = D3D11_BLEND_ZERO;
	bd0.DestBlendAlpha = D3D11_BLEND_ONE;
	bd0.BlendEnable = FALSE;
	bd0.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ComPtr<ID3D11BlendState> blendState;
	ID3D11BlendState **bs = blendState.ReleaseAndGetAddressOf();
	hr = spDevice->CreateBlendState(&blendDesc, bs);
	if (SUCCEEDED(hr)) {
		const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		spDeviceContext->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);
	} else {
		return -1;
	}

	// ds : デプスステンシルは無効扱い
	D3D11_DEPTH_STENCIL_DESC dsDesc = {0};
	dsDesc.DepthEnable = FALSE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.StencilEnable = FALSE;

	ComPtr<ID3D11DepthStencilState> depthState;
	ID3D11DepthStencilState **dspp = depthState.ReleaseAndGetAddressOf();
	hr = spDevice->CreateDepthStencilState(&dsDesc, dspp);
	if (SUCCEEDED(hr)) {
		spDeviceContext->OMSetDepthStencilState(depthState.Get(), 0);
	} else {
		return -1;
	}

	// rs : カリング無し、デプスクリップ無し、シザーあり
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.DepthClipEnable = FALSE;
	rsDesc.ScissorEnable = TRUE;

	ComPtr<ID3D11RasterizerState> rasterizerState;
	ID3D11RasterizerState **rspp = rasterizerState.ReleaseAndGetAddressOf();
	hr = spDevice->CreateRasterizerState(&rsDesc, rspp);
	if (SUCCEEDED(hr)) {
		spDeviceContext->RSSetState(rasterizerState.Get());
	} else {
		return -1;
	}

	// ビューポートをビュー全体に
	D3D11_VIEWPORT vp = {0};
	vp.Width = (float)sWidth;
	vp.Height = (float)sHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	spDeviceContext->RSSetViewports(1, &vp);

	// 画面クリア
#if 0
	static float b = 0.0f;
	b += (1.0f/60.0f);
	if (b > 1.0f) b -= 1.0f;
	float clearColor[] = { 0.0f, 0.0f, b, 0.0f };
#else
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
#endif
#if USE_SWAP_CHAIN
	spDeviceContext->ClearRenderTargetView( spRenderTargetView, clearColor );
	spDeviceContext->ClearDepthStencilView( spDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
	return 0;
#endif
}

//------------------------------------------------
int
D3DView_AfterRender(void)
{
#if USE_SWAP_CHAIN
	spSwapChain->Present(NULL, NULL);
#endif
	return 0;
}

//------------------------------------------------
int
D3DView_Debug(void)
{
	IDXGIDebug* pDxgiDebug = NULL;
	ID3D11Debug* pD3dDebug = NULL;
	HRESULT hr;

	typedef HRESULT(__stdcall *fPtr)(const IID&, void**); 
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	if (hDll) {
		fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
		if (DXGIGetDebugInterface) {
			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);
		}
	}
	if (pDxgiDebug) {
		hr = pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
		pDxgiDebug->Release();
		return 0;
	}

	spDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pD3dDebug));
	pD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	pD3dDebug->Release();

	return 0;
}

//------------------------------------------------
// [EOF]
