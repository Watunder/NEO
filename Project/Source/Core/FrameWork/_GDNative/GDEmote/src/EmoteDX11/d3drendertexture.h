//  ◇UTF-8

#ifndef __D3DRENDERTEXTURE_H__
#define __D3DRENDERTEXTURE_H__

#include <windows.h>
#include <d3d11.h>

typedef struct tagD3DRenderTexture {
	D3D11_TEXTURE2D_DESC mStageDesc;
	D3D11_TEXTURE2D_DESC mTexDesc;
	D3D11_RENDER_TARGET_VIEW_DESC mRtvDesc;
	D3D11_TEXTURE2D_DESC mTexDSDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC mDsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC mSrvDesc;
	D3D11_SAMPLER_DESC mSmpDesc;

	ID3D11Texture2D* mStage;
	ID3D11Texture2D* mTex;
	ID3D11RenderTargetView* mRtv;
	ID3D11Texture2D* mTexDS;
	ID3D11DepthStencilView *mDsv;
	ID3D11ShaderResourceView* mSrv;
	ID3D11SamplerState* mSmp;

	ID3D11RenderTargetView* mRtvBefore;
	ID3D11DepthStencilView* mDsvBefore;
	ID3D11ShaderResourceView* mSrvBefore;
} D3DRenderTexture;


D3DRenderTexture*
D3DRenderTexture_Create(int width, int height);

void
D3DRenderTexture_Destroy(D3DRenderTexture* p);

int
D3DRenderTexture_BeforeRender(D3DRenderTexture* p);

int
D3DRenderTexture_AfterRender(D3DRenderTexture* p);

int
D3DRenderTexture_Bind(D3DRenderTexture* p, int id);

int
D3DRenderTexture_Unbind(D3DRenderTexture* p, int id);


#endif  // __D3DRENDERTEXTURE_H__

// [EOF]
