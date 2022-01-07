#pragma once

#include <Godot.hpp>
#include <Viewport.hpp>

namespace godot
{
	class RmlUiMain : public Viewport
	{
		GODOT_CLASS(RmlUiMain, Viewport);

	public:
		RmlUiMain();
		~RmlUiMain();

		void _init();

		static void _register_methods();
	private:
	};
}
