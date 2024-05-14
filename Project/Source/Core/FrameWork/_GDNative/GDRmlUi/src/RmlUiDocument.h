#pragma once

#include <Godot.hpp>
#include <Resource.hpp>

namespace godot
{
	class RmlUiDocument : public Resource
	{
		GODOT_CLASS(RmlUiDocument, Resource);

	public:
		RmlUiDocument();
		~RmlUiDocument();

		void _init();

		static void _register_methods();
	private:
		PoolByteArray m_data_bytes;
	};
}