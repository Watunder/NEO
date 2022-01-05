#include "GDLibrary.h"

#include "RmlUiMain.h"
// -----------------------------------------------------------------

using namespace godot;

// -----------------------------------------------------------------

// GDNative Initialize
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o)
{
	Godot::gdnative_init(o);
}

// GDNative Terminate
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o)
{
	Godot::gdnative_terminate(o);
}

// NativeScript Initialize
extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
	Godot::nativescript_init(handle);

	register_class<RmlUiMain>();
}

// -----------------------------------------------------------------