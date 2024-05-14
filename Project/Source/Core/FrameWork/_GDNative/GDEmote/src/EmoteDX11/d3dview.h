//  ◇UTF-8

#ifndef __D3DVIEW_H__
#define __D3DVIEW_H__

#include <windows.h>
#include <d3d11.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(_a)		{ if(_a){ (_a)->Release(); (_a)=NULL; } }
#endif

IDXGISwapChain*
D3DView_SwapChain(void);

ID3D11Device*
D3DView_Device(void);

ID3D11DeviceContext*
D3DView_Context(void);

ID3D11RenderTargetView*
D3DView_RenderTextureView(void);

ID3D11DepthStencilView*
D3DView_DepthStencilView(void);

int
D3DView_CreateDevice(HWND hwnd);

int
D3DView_CreateBuffer(void);

int
D3DView_DestroyBuffer(void);

int
D3DView_DestroyDevice(void);

int
D3DView_BeforeRender(void);

int
D3DView_AfterRender(void);

int
D3DView_Debug(void);

#endif  // __D3DVIEW_H__

// [EOF]
