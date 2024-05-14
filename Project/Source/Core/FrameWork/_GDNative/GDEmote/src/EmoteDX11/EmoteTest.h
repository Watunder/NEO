//
//  ◇UTF-8
//
//  EmoteTest.h
//  emotetest
//

#ifndef __EMOTETEST_H__
#define __EMOTETEST_H__

#include "iemote.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern const wchar_t *EmoteTest_GetDataFileName(void);
extern void EmoteTest_Initialize(const IEmoteDevice::InitParam& param, const unsigned char *buf, unsigned int size);
extern void EmoteTest_Finish(void);
extern void EmoteTest_Update(float ms);
extern void EmoteTest_Draw(void);
extern void EmoteTest_Pause(void);
extern void EmoteTest_Resume(void);
extern void EmoteTest_ResetMatrix(void);
extern void EmoteTest_SetTouchMode(int mode);
extern int EmoteTest_GetTouchMode(void);
extern void EmoteTest_SetMode(int mode);
extern void EmoteTest_SetOffsetCoord(int ofsx, int ofsy);
extern void EmoteTest_SetOffsetScale(float ofstScale);
extern void EmoteTest_SetOffsetRot(float ofstRot);
extern void EmoteTest_SwitchMouth(void);
extern void EmoteTest_SwitchQueuing(void);
extern void EmoteTest_Skip(void);
extern void EmoteTest_OnTouchStart(int no, float x, float y);
extern void EmoteTest_OnTouchMove(int no, float x, float y);
extern void EmoteTest_OnTouchEnd(int no, float x, float y);
extern void EmoteTest_SetViewport(float x, float y, float w, float h);
extern void EmoteTest_SetRenderTargetInfo(ID3D11RenderTargetView* rendertarget, ID3D11DepthStencilView* depthstencil);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __EMOTETEST_H__
// [EOF]
