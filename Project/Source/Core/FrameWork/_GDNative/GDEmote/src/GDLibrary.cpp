#include "GDLibrary.h"

#include "GDEmote.h"

using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * option)
{
	Godot::gdnative_init(option);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * option)
{
	Godot::gdnative_terminate(option);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
	Godot::nativescript_init(handle);
	register_class<Emoto>();
}
