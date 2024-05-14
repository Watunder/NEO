#pragma once
#include <Godot.hpp>
#include <Node2D.hpp>

#include "d3dview.h"
#include "d3drendertexture.h"

#include "EmoteTest.h"

namespace godot
{
	class Emoto : public Node2D
	{
		GODOT_CLASS(Emoto, Node2D);

	public:

		static void _register_methods();

		Emoto();
		~Emoto();

		void _init();
		void _process(float delta);

		int _d3d_init();
		int _emote_init();
		int do_render();
		int copy_buffer();
		void add_texture();

	private:

		static const size_t TEXTURE_SIZE_X = 1024;
		static const size_t TEXTURE_SIZE_Y = 1024;

		godot::RID m_material;
		godot::RID m_canvas_item;
		godot::RID m_image_texture;
		godot::PoolByteArray m_image_data;
	};
}
