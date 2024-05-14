#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>

namespace godot
{
	class RmlUiMain : public Node2D
	{
		GODOT_CLASS(RmlUiMain, Node2D);

	public:
		static void _register_methods();

		static RmlUiMain* get_instance() { return s_instance; }

		RmlUiMain();
		~RmlUiMain();

		void _init();
	private:
		static RmlUiMain* s_instance;
	};
}
