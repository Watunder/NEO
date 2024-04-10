#ifndef TJS2PARSER_H
#define TJS2PARSER_H

#include "GDKRKR.Utils.h"
#include "GDKRKR.Wrapper.h"

// -----------------------------------------------------------------

namespace godot
{

class KrKrScript : public Reference
{
	GODOT_CLASS(KrKrScript, Reference);

public:

	KrKrScript();
	~KrKrScript();

	void _init();

	Variant evalTJS(Variant script);

	Variant execTJS(Variant script);

	static void _register_methods();

private:

	krkr::tTJSWrapper s_instance;
};

}

// -----------------------------------------------------------------

#endif
