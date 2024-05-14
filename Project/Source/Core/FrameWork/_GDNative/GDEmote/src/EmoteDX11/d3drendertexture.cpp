//  ◇UTF-8

#include <limits.h>
#include <float.h>
#include <windows.h>
#include <mmsystem.h>
#include <d3d11.h>


#pragma comment(lib,"d3d11.lib")

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "d3drendertexture.h"
#include "d3dview.h"
void printd(const char *format,...);

//------------------------------------------------

//------------------------------------------------
D3DRenderTexture*
D3DRenderTexture_Create(int width, int height)
{
	ID3D11Device* dev = D3DView_Device();
	HRESULT hr;

	D3DRenderTexture* p = new D3DRenderTexture();
	ZeroMemory(p, sizeof(D3DRenderTexture));
	
	p->mStageDesc.Usage = D3D11_USAGE_STAGING;
	p->mStageDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	p->mStageDesc.BindFlags = 0;
	p->mStageDesc.Width = width;
	p->mStageDesc.Height = height;
	p->mStageDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	p->mStageDesc.MipLevels = 1;
	p->mStageDesc.ArraySize = 1;
	p->mStageDesc.SampleDesc.Count = 1;
	p->mStageDesc.SampleDesc.Quality = 0;
	hr = dev->CreateTexture2D(&(p->mStageDesc), NULL, &(p->mStage));
	if (FAILED(hr)) {
		delete p;
		return NULL;
	}
	
	p->mTexDesc.Usage = D3D11_USAGE_DEFAULT;
	p->mTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	p->mTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	p->mTexDesc.Width = width;
	p->mTexDesc.Height = height;
	p->mTexDesc.CPUAccessFlags = 0;
	p->mTexDesc.MipLevels = 1;
	p->mTexDesc.ArraySize = 1;
	p->mTexDesc.SampleDesc.Count = 1;
	p->mTexDesc.SampleDesc.Quality = 0;
	hr = dev->CreateTexture2D(&(p->mTexDesc), NULL, &(p->mTex));
	if (FAILED(hr)) {
		delete p;
		return NULL;
	}

	p->mRtvDesc.Format = p->mTexDesc.Format;
	p->mRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	hr = dev->CreateRenderTargetView(p->mTex, &(p->mRtvDesc), &(p->mRtv));
	if (FAILED(hr)) {
		p->mTex->Release();
		delete p;
		return NULL;
	}

	p->mTexDSDesc.Usage = D3D11_USAGE_DEFAULT;
	p->mTexDSDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	p->mTexDSDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	p->mTexDSDesc.Width = width;
	p->mTexDSDesc.Height = height;
	p->mTexDSDesc.CPUAccessFlags = 0;
	p->mTexDSDesc.MipLevels = 1;
	p->mTexDSDesc.ArraySize = 1;
	p->mTexDSDesc.SampleDesc.Count = 1;
	p->mTexDSDesc.SampleDesc.Quality = 0;
	hr = dev->CreateTexture2D(&(p->mTexDSDesc), NULL, &(p->mTexDS));
	if (FAILED(hr)) {
		p->mRtv->Release();
		p->mTex->Release();
		p->mStage->Release();
		delete p;
		return NULL;
	}

	p->mDsvDesc.Format = p->mTexDSDesc.Format;
	p->mDsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	p->mDsvDesc.Texture2D.MipSlice = 0;
	hr = dev->CreateDepthStencilView(p->mTexDS, &(p->mDsvDesc), &(p->mDsv));
	if (FAILED(hr)) {
		p->mTexDS->Release();
		p->mRtv->Release();
		p->mTex->Release();
		p->mStage->Release();
		delete p;
		return NULL;
	}


	p->mSrvDesc.Format = p->mRtvDesc.Format;
	p->mSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	p->mSrvDesc.Texture2D.MipLevels = 1;
	hr = dev->CreateShaderResourceView(p->mTex, &(p->mSrvDesc), &(p->mSrv));
	if (FAILED(hr)) {
		p->mDsv->Release();
		p->mTexDS->Release();
		p->mRtv->Release();
		p->mTex->Release();
		p->mStage->Release();
		delete p;
		return NULL;
	}

	p->mSmpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	p->mSmpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	p->mSmpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	p->mSmpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	p->mSmpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	p->mSmpDesc.MinLOD = 0;
	p->mSmpDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = dev->CreateSamplerState(&(p->mSmpDesc), &(p->mSmp));
	if (FAILED(hr)) {
		p->mSrv->Release();
		p->mDsv->Release();
		p->mTexDS->Release();
		p->mRtv->Release();
		p->mTex->Release();
		p->mStage->Release();
		delete p;
		return NULL;
	}

	return p;
}

//------------------------------------------------
void
D3DRenderTexture_Destroy(D3DRenderTexture* p)
{
	p->mSmp->Release();
	p->mSrv->Release();
	p->mDsv->Release();
	p->mTexDS->Release();
	p->mRtv->Release();
	p->mTex->Release();
	p->mStage->Release();
	delete p;
}

//------------------------------------------------
int
D3DRenderTexture_BeforeRender(D3DRenderTexture* p)
{
	ID3D11Device* dev = D3DView_Device();
	ID3D11DeviceContext* ctx = D3DView_Context();
	ID3D11BlendState *bs;
	ID3D11DepthStencilState* ds;
	ID3D11RasterizerState* rs;
	HRESULT hr;

	ctx->OMGetRenderTargets(1, &(p->mRtvBefore), &(p->mDsvBefore));
	ctx->OMSetRenderTargets(1, &(p->mRtv), p->mDsv);

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
	hr = dev->CreateBlendState(&blendDesc, &bs);
	if (SUCCEEDED(hr)) {
		const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ctx->OMSetBlendState(bs, blendFactor, 0xffffffff);
	}

	// ds : デプスステンシルは有効扱い
	D3D11_DEPTH_STENCIL_DESC dsDesc = {0};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.StencilEnable = TRUE;
	dsDesc.StencilReadMask = 0xff;
	dsDesc.StencilWriteMask = 0xff;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = dev->CreateDepthStencilState(&dsDesc, &ds);
	if (SUCCEEDED(hr)) {
		ctx->OMSetDepthStencilState(ds, 0);
	}

	// rs : カリング無し、デプスクリップ無し、シザーあり
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.DepthClipEnable = FALSE;
	rsDesc.ScissorEnable = TRUE;
	hr = dev->CreateRasterizerState(&rsDesc, &rs);
	if (SUCCEEDED(hr)) {
		ctx->RSSetState(rs);
	}

	// ビューポートをビュー全体に
	D3D11_VIEWPORT vp = {0};
	vp.Width = (float)p->mTexDesc.Width;
	vp.Height = (float)p->mTexDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ctx->RSSetViewports(1, &vp);

	// 画面クリア
#if 1
	static float b = 0.0f;
	b += (1.0f/60.0f);
	if (b > 1.0f) b -= 1.0f;
	float clearColor[] = { 0.0f, 0.0f, b, 0.0f };
#else
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
#endif
	ctx->ClearRenderTargetView(p->mRtv, clearColor);
	ctx->ClearDepthStencilView(p->mDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	rs->Release();
	ds->Release();
	bs->Release();

	return 0;
}

//------------------------------------------------
int
D3DRenderTexture_AfterRender(D3DRenderTexture* p)
{
	ID3D11DeviceContext* ctx = D3DView_Context();

	ctx->OMSetRenderTargets(1, &(p->mRtvBefore), p->mDsvBefore);
	SAFE_RELEASE(p->mRtvBefore);
	SAFE_RELEASE(p->mDsvBefore);

	return 0;
}

//------------------------------------------------
int
D3DRenderTexture_Bind(D3DRenderTexture* p, int id)
{
	ID3D11DeviceContext* ctx = D3DView_Context();

	ctx->PSGetShaderResources(id, 1, &(p->mSrvBefore));

	ctx->PSSetSamplers(id, 1, &(p->mSmp));
	ctx->PSSetShaderResources(id, 1, &(p->mSrv));

	return 0;
}

//------------------------------------------------
int
D3DRenderTexture_Unbind(D3DRenderTexture* p, int id)
{
	ID3D11DeviceContext* ctx = D3DView_Context();

	ctx->PSSetShaderResources(id, 1, &(p->mSrvBefore));

	return 0;
}


//------------------------------------------------
// [EOF]
