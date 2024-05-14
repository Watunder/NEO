#include "GDEmote.h"

#include <Image.hpp>
#include <Texture.hpp>
#include <ImageTexture.hpp>
#include <VisualServer.hpp>

#pragma comment(lib,"winmm.lib")

static D3DRenderTexture* sRenderTexture;

namespace godot
{
	void Emoto::_register_methods()
	{
		register_method("_init", &Emoto::_init);
		register_method("_process", &Emoto::_process);
		register_method("do_render", &Emoto::do_render);
		register_method("_emote_init", &Emoto::_emote_init);
		register_method("add_texture", &Emoto::add_texture);
	}

	Emoto::Emoto()
	{
		auto vs = godot::VisualServer::get_singleton();

		godot::Ref<godot::Image> image;

		image.instance();
		image->create(TEXTURE_SIZE_X, TEXTURE_SIZE_Y, false, godot::Image::FORMAT_RGBA8);

		m_image_texture = vs->texture_create_from_image(image, 0);
		m_canvas_item = vs->canvas_item_create();
		m_material = vs->material_create();
	}

	Emoto::~Emoto()
	{
		EmoteTest_Finish();
		D3DRenderTexture_Destroy(sRenderTexture);

		D3DView_DestroyDevice();
	}

	void Emoto::_init()
	{
		_d3d_init();
		_emote_init();
	}

	void Emoto::_process(float delta)
	{
		EmoteTest_Update(delta * 1000);
		do_render();
	}

	int Emoto::_d3d_init()
	{
		int ret;

		ret = D3DView_CreateDevice(NULL);
		if (ret < 0) {
			return -1;
		}
		sRenderTexture = D3DRenderTexture_Create(TEXTURE_SIZE_X, TEXTURE_SIZE_Y);

		return 0;
	}

	int Emoto::_emote_init()
	{
		const wchar_t* wfilename = EmoteTest_GetDataFileName();
		setlocale(LC_ALL, "japanese");

		HANDLE handle = CreateFile(
			wfilename,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (handle == INVALID_HANDLE_VALUE) {
			return -1;
		}

		emote_uint32_t size = GetFileSize(handle, NULL);
		emote_uint8_t* buf = new emote_uint8_t[size];
		DWORD readed;
		ReadFile(handle, buf, size, &readed, NULL);
		CloseHandle(handle);

		IEmoteDevice::InitParam param = { 0 };
		param.device = D3DView_Device();
		param.context = D3DView_Context();
		param.renderTargetView = sRenderTexture->mRtv;
		param.depthStencilView = sRenderTexture->mDsv;

		EmoteTest_Initialize(param, buf, size);
		EmoteTest_SetViewport(0, 0, TEXTURE_SIZE_X, TEXTURE_SIZE_Y);
		delete[] buf;

		return 0;
	}

	int Emoto::do_render()
	{
		D3DRenderTexture_BeforeRender(sRenderTexture);
		EmoteTest_SetViewport(0, 0, TEXTURE_SIZE_X, TEXTURE_SIZE_Y);

		EmoteTest_Draw();
		copy_buffer();
		D3DRenderTexture_AfterRender(sRenderTexture);

		D3DView_BeforeRender();

		return 0;
	}

	int Emoto::copy_buffer()
	{
		D3DView_Context()->CopyResource(sRenderTexture->mStage, sRenderTexture->mTex);

		D3D11_MAPPED_SUBRESOURCE ResourceDesc = {};

		D3DView_Context()->Map(sRenderTexture->mStage, 0, D3D11_MAP_READ, 0, &ResourceDesc);

		if (ResourceDesc.pData)
		{
			m_image_data.resize(TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4);
			uint8_t* dptr = m_image_data.write().ptr();
			const uint8_t* sptr = static_cast<const uint8_t*>(ResourceDesc.pData);

			for (size_t h = 0; h < TEXTURE_SIZE_Y; ++h)
			{
				std::memcpy(dptr, sptr, TEXTURE_SIZE_X * 4);
				dptr += TEXTURE_SIZE_X * 4;
				sptr += ResourceDesc.RowPitch;
			}
		}

		D3DView_Context()->Unmap(sRenderTexture->mStage, 0);

		add_texture();

		return 0;
	}

	void Emoto::add_texture()
	{
		auto vs = godot::VisualServer::get_singleton();

		vs->canvas_item_clear(m_canvas_item);
		vs->canvas_item_set_parent(m_canvas_item, godot::RID());

		m_canvas_item = vs->canvas_item_create();

		godot::Ref<godot::Image> image;
		image.instance();
		
		image->create_from_data(TEXTURE_SIZE_X, TEXTURE_SIZE_Y, false, godot::Image::FORMAT_RGBA8, m_image_data);

		vs->texture_set_data(m_image_texture, image, 0);

		vs->canvas_item_add_texture_rect(m_canvas_item, godot::Rect2(-0.5, -0.5, TEXTURE_SIZE_X, TEXTURE_SIZE_Y), m_image_texture);

		vs->canvas_item_set_parent(m_canvas_item, get_canvas_item());
		vs->canvas_item_set_material(m_canvas_item, m_material);
	}
}
