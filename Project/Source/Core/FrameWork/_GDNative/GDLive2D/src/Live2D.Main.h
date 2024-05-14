#pragma once

#include <Godot.hpp>

namespace godot {

	class Live2DMain : public Reference
	{
		GODOT_CLASS(Live2DMain, Reference)

	public:
		static void _register_methods();

		Live2DMain();

		~Live2DMain();

		void _init();
	};
}
