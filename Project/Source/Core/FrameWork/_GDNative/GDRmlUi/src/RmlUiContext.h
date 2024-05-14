#pragma once

#include "RmlUi/Core/Context.h"

#include <Godot.hpp>
#include <Control.hpp>

namespace godot
{
	class RmlUiContext : public Control
	{
		GODOT_CLASS(RmlUiContext, Control);

	public:
		static void _register_methods();

		RmlUiContext();
		~RmlUiContext();

		void _init();
		
		void _update();
		
		void _render();
	private:
		Rml::Context* m_context = nullptr;
	};
}