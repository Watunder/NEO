#ifndef KRKRGODOT_WRAPPER_H
#define KRKRGODOT_WRAPPER_H

#include "GDKRKR.Utils.h"

// -----------------------------------------------------------------

namespace krkr
{

class iTJSDispatch2Wrapper : public tTJSDispatch
{
protected:

	godot::Object gd_obj;

public:

	iTJSDispatch2Wrapper(godot::Object gd_obj);
	~iTJSDispatch2Wrapper();

	tjs_error TJS_INTF_METHOD
	CreateNew(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, iTJSDispatch2** result, tjs_int numparams, tTJSVariant** param, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	FuncCall(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, tTJSVariant* result, tjs_int numparams, tTJSVariant** param, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	FuncCallByNum(tjs_uint32 flag, tjs_int num, tTJSVariant* result, tjs_int numparams, tTJSVariant** param, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	PropGet(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, tTJSVariant* result, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	PropGetByNum(tjs_uint32 flag, tjs_int num, tTJSVariant* result, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	PropSet(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, const tTJSVariant* param, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	PropSetByNum(tjs_uint32 flag, tjs_int num, const tTJSVariant* param, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	EnumMembers(tjs_uint32 flags, tTJSVariantClosure* callback, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	DeleteMember(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	DeleteMemberByNum(tjs_uint32 flag, tjs_int num, iTJSDispatch2* objthis);

	tjs_error TJS_INTF_METHOD
	IsInstanceOf(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, const tjs_char* classname, iTJSDispatch2* objthis);
};

// -----------------------------------------------------------------

class tTJSDispatch2Wrapper
{
protected:

	iTJSDispatch2* dispatch;
	iTJSDispatch2* dispatch_this;

public:

	tTJSDispatch2Wrapper(iTJSDispatch2* in_dispatch, iTJSDispatch2* in_dispatch_this = NULL);
	~tTJSDispatch2Wrapper(void);
	iTJSDispatch2* GetNoAddRef(void);
	iTJSDispatch2* GetThisNoAddRef(void);
};

// -----------------------------------------------------------------

class tTJSWrapper
{
protected:

	tTJS* ScriptEngine;

public:

	tTJSWrapper();

	~tTJSWrapper();
	
	tTJSDispatch2Wrapper* GetGlobal();

	godot::Variant
	ExecScript(godot::Variant script, godot::Object context, godot::Variant name, godot::Variant lineofs);

	godot::Variant
	EvalExpression(godot::Variant script, godot::Object context, godot::Variant name, godot::Variant lineofs);

	godot::Variant
	LoadByteCode(godot::Variant buff, godot::Object context, godot::Variant name);

	godot::Variant
	CompileScript(godot::Variant script, godot::Variant isresultneeded, godot::Variant outputdebug, godot::Variant isexpression, godot::Variant name, godot::Variant lineofs);

	void
	SetPPValue(const tjs_char* name, const tjs_int32 value);

	tjs_int32
	GetPPValue(const tjs_char* name);

	void
	DoGarbageCollection(void);
};

}

// -----------------------------------------------------------------

#endif