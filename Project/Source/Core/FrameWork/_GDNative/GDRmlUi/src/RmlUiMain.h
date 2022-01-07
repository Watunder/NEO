#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>

namespace godot
{
	class RmlUiMain : public Node2D
	{
		GODOT_CLASS(RmlUiMain, Node2D);

	public:
		RmlUiMain();
		~RmlUiMain();

		void _init();

		static void _register_methods();
	private:
	};
}
