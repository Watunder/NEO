#pragma once

#include <Godot.hpp>
#include <Reference.hpp>

#include "iemote.h"

namespace godot
{
	class Emote : public Reference
	{
		GODOT_CLASS(Emote, Reference);

	public:
		void _init();
		Emote();
		~Emote();

		static void _register_methods();
	};
}
