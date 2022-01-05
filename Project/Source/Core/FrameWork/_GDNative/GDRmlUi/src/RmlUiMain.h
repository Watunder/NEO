#pragma once

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class RmlUiMain : public Reference
	{
		GODOT_CLASS(RmlUiMain, Reference);

	public:
		RmlUiMain();
		~RmlUiMain();

		void _init();

		static void _register_methods();
	private:
	};
}
