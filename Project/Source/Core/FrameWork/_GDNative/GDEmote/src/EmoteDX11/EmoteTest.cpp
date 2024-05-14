//
//  ��SHIFT-JIS
//
//  Motion.cpp
//  emotetest
//

#include <iemote.h>
#include "EmoteTest.h"
#include <stdio.h>
#include <windows.h>
// #include <glAscii.h>

//------------------------------------------------
// �萔��`
//------------------------------------------------

//------------------------------------------------
// �ϊ�
static const float MSTOF60THS = 60.0f / 1000.0f;  // ms��1/60�b�J�E���g�֕ϊ��B
static const float F60THSTOMS = 1000.0f / 60.0f;  // 1/60�b�J�E���g��ms�֕ϊ��B

//------------------------------------------------
// E-mote�f�[�^�t�@�C��
static const wchar_t* MOTION_DATA_PATH = L"models/emote_test.psb";

//------------------------------------------------
// Emote�֘A
IEmoteDevice* sEmoteDevice;
IEmotePlayer* sEmotePlayer;

//------------------------------------------------
// �֐��錾
//------------------------------------------------
struct VariableTable
{
    const char* key;
    float value;
};

static void EmoteInit(const IEmoteDevice::InitParam& param, const unsigned char* buf, unsigned int size);
static void EmoteDone(void);
static void EmoteUpdate(float ms);
static void EmoteDraw(void);
static void EmotePause(void);
static void EmoteResume(void);
static void EmoteOffsetCoord(int ofsx, int ofsy);
static void EmoteOffsetScale(float ofstScale);
static void EmoteOffsetRot(float ofstRot);
static void EmoteResetMatrix(void);
static void EmoteSetVariables(const VariableTable* table, float time, float easing);
static void EmoteUpdatePoseTimeline(int newIndex);
static void EmoteUpdateMouthTimeline(int newIndex);
static void EmoteSkip(void);
static void EmoteSetViewport(float x, float y, float w, float h);
static void EmoteSetRenderTargetInfo(ID3D11RenderTargetView* rendertarget, ID3D11DepthStencilView* depthstencil);

//------------------------------------------------
// �ϐ��ݒ�̃Z�b�g
//------------------------------------------------
static const char* sPoseTimelineList[] = {
  "test00",
  "test01",
  "test02",
  "test03"
};
static uint32_t sPoseTableSize = sizeof(sPoseTimelineList) / sizeof(sPoseTimelineList[0]);
static int32_t sPoseIndex = 0;

static const char* sMouthTimelineList[] = {
  "mouth_test00",
  "mouth_test01"
};
static uint32_t sMouthTableSize = sizeof(sMouthTimelineList) / sizeof(sMouthTimelineList[0]);
static int32_t sMouthIndex = 0;

static const VariableTable
sFaceTable0[] =
{
  { "face_eye_UD", 0.0 },
  { "face_eye_LR", 0.0 },
  { "face_eye_open", 0.0 },
  { "face_eyebrow", 0.0 },
  { "face_mouth", 0.0 },
  { "face_talk", 0.0 },
  { "face_tears", 0.0 },
  { "face_cheek", 0.0} ,
  {}
};

static const VariableTable
sFaceTable1[] =
{
  { "face_eye_UD", 0.0 },
  { "face_eye_LR", -30 },
  { "face_eye_open", 0.0 },
  { "face_eyebrow", 40 },
  { "face_mouth", 30 },
  { "face_talk", 0.0 },
  { "face_tears", 0.0 },
  { "face_cheek", 1 },
  {},
};

static const VariableTable
sFaceTable2[] =
{
  { "face_eye_UD", 0.0 },
  { "face_eye_LR", -30.0 },
  { "face_eye_open", 10 },
  { "face_eyebrow",  30 },
  { "face_mouth", 20 },
  { "face_talk", 0.0 },
  { "face_tears", 1 },
  { "face_cheek", 0.0 },
  {},
};

static const VariableTable
sFaceTable3[] =
{
  { "face_eye_UD", -30 },
  { "face_eye_LR", 0.0 },
  { "face_eye_open", 5 },
  { "face_eyebrow", 20 },
  { "face_mouth", 20 },
  { "face_talk", 0.5 },
  { "face_tears", 0.0 },
  { "face_cheek", 0.0 },
  {},
};

static const VariableTable*
sFaceTable[] = {
  sFaceTable0,
  sFaceTable1,
  sFaceTable2,
  sFaceTable3,
};
static uint32_t sFaceTableSize = sizeof(sFaceTable) / sizeof(sFaceTable[0]);
static int32_t sFaceIndex = 0;

typedef enum {
    TOUCHMODE_NONE = 0,
    TOUCHMODE_COORD,
    TOUCHMODE_SCALE,
    TOUCHMODE_ROT,
    TOUCHMODE_COMMAND,
} TOUCHMODE;

typedef enum {
    TOUCHCOMMAND_NONE = 0,
    TOUCHCOMMAND_POSE_0,
    TOUCHCOMMAND_POSE_1,
    TOUCHCOMMAND_POSE_2,
    TOUCHCOMMAND_POSE_3,
    TOUCHCOMMAND_FACE_0,
    TOUCHCOMMAND_FACE_1,
    TOUCHCOMMAND_FACE_2,
    TOUCHCOMMAND_FACE_3,
    TOUCHCOMMAND_SWITCHMOUTH,
    TOUCHCOMMAND_SKIP,
    TOUCHCOMMAND_RESET,
} TOUCHCOMMAND;

static TOUCHMODE sTouchMode;
static TOUCHCOMMAND sTouchCommand;
static float sTouchLastX;
static float sTouchLastY;
static bool sMoveStart;
static int sLastFps = 0;

//------------------------------------------------
// E-mote����
//------------------------------------------------

static void myDrawText(const wchar_t* format, ...)
{
    va_list ap;
    wchar_t buf[2048];
    va_start(ap, format);
    _vsnwprintf_s(buf, 2048, format, ap);
    va_end(ap);

#if 0
    HWND hwnd = GetActiveWindow();
    HDC hdc = GetDC(hwnd);
    RECT rt = { 0, 0, 512, 24 };
    DrawText(hdc, buf, strlen(buf), &rt, DT_LEFT | DT_SINGLELINE | DT_TOP | DT_HIDEPREFIX);
    ReleaseDC(hwnd, hdc);
#else
    HWND hwnd = GetActiveWindow();
    SetWindowText(hwnd, buf);
#endif
}

//------------------------------------------------
// FPS�\��
static void CalcFps(float ms)
{
    static float sLastTimeMs = 0.0f;
    static float sNowTimeMs = 0.0f;
    static int sFps = 0;

    sFps++;
    sLastTimeMs += ms;
    if (sLastTimeMs >= 1000.0f) {
        sLastFps = sFps;
        sFps = 0;
        sLastTimeMs -= 1000.0f;
    }
}

static void PrintFps(void)
{
    // glAsciiPrintf(0, 2, 2, "Fps = %d", sLastFps );
}

//------------------------------------------------
// �O���C�X�P�[���t�B���^
static void GrayscaleFilter(emote_uint8_t* image, emote_uint32_t imageSize)
{
    while (imageSize) {
        // TODO RGB ���я�
        emote_uint8_t gray = int(0.298912f * image[2] + 0.586611f * image[1] + 0.114478f * image[0]);
        image[0] = image[1] = image[2] = gray;
        image += 4;
        imageSize -= 4;
    }
}

//------------------------------------------------
// ������
static void EmoteInit(const IEmoteDevice::InitParam& param, const unsigned char* buf, unsigned int size)
{
    // E-mote�f�o�C�X�쐬
    sEmoteDevice = EmoteCreate(param);

#if 0
    // E-mote�t�@�C���C���[�W�Ɏ��O�Ƀt�B���^�������s��
    EmoteFilterTexture(buf, size, GrayscaleFilter);
#endif

    // E-mote�v���C���쐬
    sEmoteDevice->CreatePlayer(buf, size, &sEmotePlayer);

    // �\���J�n
    sEmotePlayer->Show();
}

//------------------------------------------------
// �I��
static void EmoteDone(void)
{
    sEmotePlayer->Release();
    sEmoteDevice->Release();
}

//------------------------------------------------
// �t���[���X�V
static void EmoteUpdate(float ms)
{
    // E-mote��1/60�b��1�P�ʂŋ쓮����̂Ŏ��ԒP�ʂ�ϊ��B
    sEmotePlayer->Progress(ms * MSTOF60THS);
}

//------------------------------------------------
// �`��
static void EmoteDraw(void)
{
    sEmotePlayer->Render();
}

//------------------------------------------------
// �|�[�Y
static void EmotePause(void)
{
    sEmoteDevice->OnPause();
}

//------------------------------------------------
// ���W���[��
static void EmoteResume(void)
{
    sEmoteDevice->OnResume();
}

//------------------------------------------------
// ���W�̃I�t�Z�b�g����
static void EmoteOffsetCoord(int ofsx, int ofsy)
{
    float x, y;
    sEmotePlayer->GetCoord(x, y);
    sEmotePlayer->SetCoord(x + ofsx, y + ofsy);
}

//------------------------------------------------
// �X�P�[���̃I�t�Z�b�g����
static void EmoteOffsetScale(float ofstScale)
{
    float scale;
    scale = sEmotePlayer->GetScale();
    sEmotePlayer->SetScale(scale * ofstScale);
}

//------------------------------------------------
// ��]�̃I�t�Z�b�g����
static void EmoteOffsetRot(float ofstRot)
{
    float rot;
    rot = sEmotePlayer->GetRot();
    sEmotePlayer->SetRot(rot + ofstRot);
}

//------------------------------------------------
// ���W�n�̃��Z�b�g
static void EmoteResetMatrix(void)
{
    sEmotePlayer->SetCoord(0.0f, 0.0f);
    sEmotePlayer->SetScale(0.4f);
    sEmotePlayer->SetRot(0.0f);
}

//------------------------------------------------
// �ϐ��̒l�X�V
static void EmoteSetVariables(const VariableTable* table, float time, float easing)
{
    while (table->key) {
        // E-mote��1/60�b��1�P�ʂŋ쓮����̂Ŏ��ԒP�ʂ�ϊ��B
        sEmotePlayer->SetVariable(table->key, table->value, time * MSTOF60THS, easing);
        table++;
    }
    printf("emote update variables.\n");
}

//------------------------------------------------
// �|�[�Y�̃^�C�����C�����X�V
void
EmoteUpdatePoseTimeline(int newIndex)
{
    sEmotePlayer->StopTimeline(sPoseTimelineList[sPoseIndex]);
    sPoseIndex = newIndex;
    sEmotePlayer->PlayTimeline(sPoseTimelineList[sPoseIndex], IEmotePlayer::TIMELINE_PLAY_PARALLEL);
    printf("emote update pose timeline.\n");
}

//------------------------------------------------
// ���̃^�C�����C�����X�V
void
EmoteUpdateMouthTimeline(int newIndex)
{
    sEmotePlayer->StopTimeline(sMouthTimelineList[sMouthIndex]);
    sMouthIndex = newIndex;
    sEmotePlayer->PlayTimeline(sMouthTimelineList[sMouthIndex], IEmotePlayer::TIMELINE_PLAY_PARALLEL);
    printf("emote update mouth timeline.\n");
}

//------------------------------------------------
// �X�L�b�v����
static void EmoteSkip(void)
{
    sEmotePlayer->Skip();
    printf("emote skip.\n");
}

//------------------------------------------------
// �r���[�|�[�g�ݒ�
static void EmoteSetViewport(float x, float y, float w, float h)
{
#if RENDERTARGET_MODE_DX  // MODE_DX
    sEmoteDevice->SetViewportInfo((emote_uint32_t)x, (emote_uint32_t)y, (emote_uint32_t)w, (emote_uint32_t)h, IEmoteDevice::VP_MODE_DX);
#else  // MODE_GL
    sEmoteDevice->SetViewportInfo((emote_uint32_t)x, (emote_uint32_t)y, (emote_uint32_t)w, (emote_uint32_t)h, IEmoteDevice::VP_MODE_GL);
#endif
}

//------------------------------------------------
// �����_�[�^�[�Q�b�g�̐ݒ�

static void EmoteSetRenderTargetInfo(ID3D11RenderTargetView* rendertarget, ID3D11DepthStencilView* depthstencil)
{
    sEmoteDevice->SetRenderTargetInfo(rendertarget, depthstencil);
}


// =====================================================================

//------------------------------------------------
// �f�[�^�t�@�C�����̎擾

const wchar_t* EmoteTest_GetDataFileName(void)
{
    return MOTION_DATA_PATH;
}

//------------------------------------------------
// ������

void EmoteTest_Initialize(const IEmoteDevice::InitParam& param, const unsigned char* buf, unsigned int size)
{
    EmoteInit(param, buf, size);

#if 1
    // �}�X�N���[�h���A���t�@�}�X�N��
    // main.c : myEglChooseConfig()�Ăяo����stencil������0�ɂ���ƁA�������ł��܂��B
    sEmoteDevice->SetMaskMode(IEmoteDevice::MASK_MODE_ALPHA);
#else
    // �}�X�N���[�h���X�e���V����
    // main.c : myEglChooseConfig()�Ăяo����stencil������8�ɂ��Ȃ��ƁA���퓮�삵�܂���B
    sEmoteDevice->SetMaskMode(IEmoteDevice::MASK_MODE_STENCIL);
#endif

    sTouchMode = TOUCHMODE_COORD;
    sTouchCommand = TOUCHCOMMAND_NONE;

    EmoteTest_ResetMatrix();
}

//------------------------------------------------
// �I��

void EmoteTest_Finish(void)
{
    EmoteDone();
}

//------------------------------------------------
// �X�V

void EmoteTest_Update(float ms)
{
    EmoteUpdate(ms);
    CalcFps(ms);
}

//------------------------------------------------
// �`��

void EmoteTest_Draw(void)
{
    float f1, f2;
    EmoteDraw();
    PrintFps();

    switch (sTouchMode) {
    case TOUCHMODE_COORD:
        sEmotePlayer->GetCoord(f1, f2);
        myDrawText(L"Swipe to move COORD: %.3f, %.3f", f1, f2);
        break;
    case TOUCHMODE_SCALE:
        f1 = sEmotePlayer->GetScale();
        myDrawText(L"Swipe to move SCALE: %f", f1);
        break;
    case TOUCHMODE_ROT:
        f1 = sEmotePlayer->GetRot();
        myDrawText(L"Swipe to move ROT: %f", f1);
        break;
    case TOUCHMODE_COMMAND:
        switch (sTouchCommand) {
        default:
        case TOUCHCOMMAND_NONE:
            myDrawText(L"Touch to COMMAND:");
            break;
        case TOUCHCOMMAND_POSE_0:
            myDrawText(L"Touch to COMMAND: POSE 0");
            break;
        case TOUCHCOMMAND_POSE_1:
            myDrawText(L"Touch to COMMAND: POSE 1");
            break;
        case TOUCHCOMMAND_POSE_2:
            myDrawText(L"Touch to COMMAND: POSE 2");
            break;
        case TOUCHCOMMAND_POSE_3:
            myDrawText(L"Touch to COMMAND: POSE 3");
            break;
        case TOUCHCOMMAND_FACE_0:
            myDrawText(L"Touch to COMMAND: FACE 0");
            break;
        case TOUCHCOMMAND_FACE_1:
            myDrawText(L"Touch to COMMAND: FACE 1");
            break;
        case TOUCHCOMMAND_FACE_2:
            myDrawText(L"Touch to COMMAND: FACE 2");
            break;
        case TOUCHCOMMAND_FACE_3:
            myDrawText(L"Touch to COMMAND: FACE 3");
            break;
        case TOUCHCOMMAND_SWITCHMOUTH:
            myDrawText(L"Touch to COMMAND: MOUTH");
            break;
        case TOUCHCOMMAND_SKIP:
            myDrawText(L"Touch to COMMAND: SKIP");
            break;
        case TOUCHCOMMAND_RESET:
            myDrawText(L"Touch to COMMAND: RESET");
            break;
        }
    }
}

//------------------------------------------------
// �|�[�Y

void EmoteTest_Pause(void)
{
    EmotePause();
}

//------------------------------------------------
// ���W���[��

void EmoteTest_Resume(void)
{
    EmoteResume();
}

//------------------------------------------------
// �}�g���N�X�̃��Z�b�g

void EmoteTest_ResetMatrix(void)
{
    EmoteResetMatrix();
}

//------------------------------------------------
// ���샂�[�h�̐ݒ�

void EmoteTest_SetTouchMode(int mode)
{
    if (mode < TOUCHMODE_COORD) mode = TOUCHMODE_COORD;
    if (mode > TOUCHMODE_COMMAND) mode = TOUCHMODE_COORD;
    sTouchMode = (TOUCHMODE)mode;
    sTouchCommand = TOUCHCOMMAND_NONE;
}

//------------------------------------------------
// ���샂�[�h�̓ǂݏo��

int EmoteTest_GetTouchMode(void)
{
    return (int)sTouchMode;
}

//------------------------------------------------
// ���[�h�̐ݒ�

void EmoteTest_SetMode(int mode)
{
    switch (mode) {
    case 0: EmoteUpdatePoseTimeline(0); break;
    case 1: EmoteUpdatePoseTimeline(1); break;
    case 2: EmoteUpdatePoseTimeline(2); break;
    case 3: EmoteUpdatePoseTimeline(3); break;
    case 4: EmoteSetVariables(sFaceTable[0], 150, 0); break;
    case 5: EmoteSetVariables(sFaceTable[1], 150, 0); break;
    case 6: EmoteSetVariables(sFaceTable[2], 150, 0); break;
    case 7: EmoteSetVariables(sFaceTable[3], 150, 0); break;
    }
}

//------------------------------------------------
// ���W�̐ݒ�

void EmoteTest_SetOffsetCoord(int ofsx, int ofsy)
{
    EmoteOffsetCoord(ofsx, ofsy);
}

//------------------------------------------------
// �X�P�[���̐ݒ�

void EmoteTest_SetOffsetScale(float ofstScale)
{
    EmoteOffsetScale(ofstScale);
}

//------------------------------------------------
// ��]�̐ݒ�

void EmoteTest_SetOffsetRot(float ofstRot)
{
    EmoteOffsetRot(ofstRot);
}

//------------------------------------------------
// ���̃X�C�b�`

void EmoteTest_SwitchMouth(void)
{
    EmoteUpdateMouthTimeline((sMouthIndex + 1) % sMouthTableSize);
}

//------------------------------------------------
// �X�L�b�v

void EmoteTest_Skip(void)
{
    EmoteSkip();
}

//------------------------------------------------
// �^�b�`�p�l���̃^�b�`�J�n
// @param no �^�b�`�ԍ�
// @param x,y ���W ����=(0.0, 0.0) - �E��(1.0, 1.0)

void EmoteTest_OnTouchStart(int no, float x, float y)
{
    //  printf("emote OnTouchStart no=%d, x=%f, y=%f\n", no, x, y);
    if (no != 0) return;

    if ((y * 5 >= 4) || (y * 5 < 1)) {
        // ��ʏ㉺���̓��[�h�؂�ւ�
        sTouchMode = (TOUCHMODE)(int(sTouchMode) + 1);
        sTouchCommand = TOUCHCOMMAND_NONE;
        if (sTouchMode > TOUCHMODE_COMMAND) {
            sTouchMode = TOUCHMODE_COORD;
        }
        sMoveStart = false;
    }
    else {
        // �������̓X���C�v���R�}���h
        sMoveStart = true;
        float vecx = (x - 0.5);
        float vecy = (y - 0.5);
        int cmd = ((int)((y * 5) - 1) * 4) + ((int)(x * 4));
        switch (sTouchMode) {
        case TOUCHMODE_COMMAND:
            switch (cmd) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                EmoteTest_SetMode(cmd);
                sTouchCommand = (TOUCHCOMMAND)(cmd + 1);
                break;
            case 8:
                EmoteTest_SwitchMouth();
                sTouchCommand = TOUCHCOMMAND_SWITCHMOUTH;
                break;
            case 9:
                EmoteTest_Skip();
                sTouchCommand = TOUCHCOMMAND_SKIP;
                break;
            case 11:
                EmoteTest_ResetMatrix();
                sTouchCommand = TOUCHCOMMAND_RESET;
                break;
            }
            break;
        case TOUCHMODE_COORD:
        case TOUCHMODE_SCALE:
        case TOUCHMODE_ROT:
            sTouchLastX = x;
            sTouchLastY = y;
            break;
        }
    }

}

//------------------------------------------------
// �^�b�`�p�l���̃^�b�`�ړ�
// @param no �^�b�`�ԍ�
// @param x,y ���W ����=(0.0, 0.0) - �E��(1.0, 1.0)

void EmoteTest_OnTouchMove(int no, float x, float y)
{
    //  printf("emote OnTouchMove no=%d, x=%f, y=%f\n", no, x, y);
    if (no != 0) return;

    if (sMoveStart) {
        switch (sTouchMode) {
        case TOUCHMODE_COORD:
            EmoteOffsetCoord((x - sTouchLastX) * 1000, (y - sTouchLastY) * 1000);
            break;
        case TOUCHMODE_SCALE:
            EmoteOffsetScale(1.0f + (y - sTouchLastY) * 10);
            break;
        case TOUCHMODE_ROT:
            EmoteOffsetRot((x - sTouchLastX) * 10);
            break;
        }
        sTouchLastX = x;
        sTouchLastY = y;
    }
}

//------------------------------------------------
// �^�b�`�p�l���̃^�b�`�I��
// @param no �^�b�`�ԍ�
// @param x,y ���W ����=(0.0, 0.0) - �E��(1.0, 1.0)

void EmoteTest_OnTouchEnd(int no, float x, float y)
{
    //  printf("emote OnTouchEnd no=%d, x=%f, y=%f\n", no, x, y);
}

//------------------------------------------------
// ���W�̐ݒ�

void EmoteTest_SetViewport(float x, float y, float w, float h)
{
    EmoteSetViewport(x, y, w, h);
}

//------------------------------------------------
// �����_�[�^�[�Q�b�g�̐ݒ�

void EmoteTest_SetRenderTargetInfo(ID3D11RenderTargetView* rendertarget, ID3D11DepthStencilView* depthstencil)
{
    EmoteSetRenderTargetInfo(rendertarget, depthstencil);
}

// =====================================================================


// [EOF]
