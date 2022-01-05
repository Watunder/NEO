#include "KRScript.h"

// -----------------------------------------------------------------

namespace godot
{

KrKrScript::KrKrScript()
{
	// init value
	// init method
	_init();
}

KrKrScript::~KrKrScript(){}

void KrKrScript::_init(){}

Variant KrKrScript::evalTJS(Variant script)
{
	Variant result = s_instance.EvalExpression(Variant(script), Object(), Variant(), Variant());

	return result;
}

Variant KrKrScript::execTJS(Variant script)
{
	Variant result = s_instance.ExecScript(Variant(script), Object(), Variant(), Variant());

	return result;
}

/*
 * register_method("method", &TJS2Parser::method);
 * register_property<TJS2Parser, String>("name", &TJS2Parser::_name, String("TJS2Parser"));
 * register_property<TJS2Parser, int>("value", &TJS2Parser::set_value, &TJS2Parser::get_value, 0);
 * register_signal<TJS2Parser>("signal_name1", "string_argument", GODOT_VARIANT_TYPE_STRING);
**/

void KrKrScript::_register_methods()
{
	register_method("evalTJS", &KrKrScript::evalTJS);
	register_method("execTJS", &KrKrScript::execTJS);
}

}

// -----------------------------------------------------------------