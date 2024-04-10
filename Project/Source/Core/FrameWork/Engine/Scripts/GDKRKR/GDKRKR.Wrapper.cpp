#include "GDKRKR.Wrapper.h"

// -----------------------------------------------------------------

namespace krkr
{

iTJSDispatch2Wrapper::iTJSDispatch2Wrapper(godot::Object gd_obj) {
    this->gd_obj = gd_obj;
}

iTJSDispatch2Wrapper::~iTJSDispatch2Wrapper() {

}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::CreateNew(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, iTJSDispatch2** result, tjs_int numparams, tTJSVariant** param, iTJSDispatch2* objthis) {
    
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::FuncCall(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, tTJSVariant* result, tjs_int numparams, tTJSVariant** param, iTJSDispatch2* objthis) {

    godot::Array gd_args;
    godot::Variant gd_result;

    for (tjs_int i = 0; i < numparams; i += 1) {
        gd_args.append(Variant_TJStoGD(*param[i]));
    }

    if (membername) {
        gd_result = gd_obj.call(Variant_TJStoGD(membername), gd_args);
    }

    iTJSDispatch2* tjs_result = Variant_GDtoTJS(gd_result);
    if (result) *result = tjs_result;;
    return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::FuncCallByNum(tjs_uint32 flag, tjs_int num, tTJSVariant* result, tjs_int numparams, tTJSVariant** param, iTJSDispatch2* objthis) {

    godot::Array gd_args;
    godot::Variant gd_result;

    for (tjs_int i = 0; i < numparams; i += 1) {
        gd_args.append(Variant_TJStoGD(*param[i]));
    }

    gd_result = gd_obj.call(Variant_TJStoGD(num), gd_args);

    tTJSVariant tjs_result = Variant_GDtoTJS(gd_result);
    if (result) *result = tjs_result;
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::PropGet(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, tTJSVariant* result, iTJSDispatch2* objthis) {

    godot::Variant gd_prop;

    if (!membername) {
        return TJS_E_NOTIMPL;
    }

    gd_prop = gd_obj.get(Variant_TJStoGD(membername));

    tTJSVariant tjs_result = Variant_GDtoTJS(gd_prop);
    if (result) *result = tjs_result;
    return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::PropGetByNum(tjs_uint32 flag, tjs_int num, tTJSVariant* result, iTJSDispatch2* objthis) {

    godot::Variant gd_prop;

    if (!num) {
        return TJS_E_NOTIMPL;
    }

    gd_prop = gd_obj.get(Variant_TJStoGD(num));

    tTJSVariant tjs_result = Variant_GDtoTJS(gd_prop);
    if (result) *result = tjs_result;
    return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::PropSet(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, const tTJSVariant* param, iTJSDispatch2* objthis) {

    godot::Variant gd_arg;

    if (!membername) {
        return TJS_E_NOTIMPL;
    }

    gd_obj.set(Variant_TJStoGD(membername), Variant_TJStoGD(param));

    return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::PropSetByNum(tjs_uint32 flag, tjs_int num, const tTJSVariant* param, iTJSDispatch2* objthis) {

    godot::Variant gd_arg;

    if (!num) {
        return TJS_E_NOTIMPL;
    }

    gd_obj.set(Variant_TJStoGD(num), Variant_TJStoGD(param));

	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::EnumMembers(tjs_uint32 flags, tTJSVariantClosure* callback, iTJSDispatch2* objthis) {

    godot::Array gd_props = gd_obj.get_property_list();
    tjs_int length = gd_props.count(gd_props);
    tTJSVariant name;
    tTJSVariant newflags = 0;
    tTJSVariant value;
    tTJSVariant* params[3] = { &name, &newflags, &value };
    for (tjs_int i = 0; i < length; i += 1)
    {
        name = Variant_GDtoTJS(gd_props[i]);
        if (!(flags & TJS_ENUM_NO_VALUE))
        {
            value = Variant_GDtoTJS(gd_obj.get(gd_props[i]));
        }
        tTJSVariant res;
        callback->FuncCall(0, NULL, NULL, &res, (flags & TJS_ENUM_NO_VALUE) ? 2 : 3, params, NULL);
    }
    return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::DeleteMember(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, iTJSDispatch2* objthis) {

    // cannot change delete object prop
    gd_obj.set(Variant_TJStoGD(membername), NULL);

	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::DeleteMemberByNum(tjs_uint32 flag, tjs_int num, iTJSDispatch2* objthis) {

    // cannot change delete object prop
    gd_obj.set(Variant_TJStoGD(num), NULL);

	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD
iTJSDispatch2Wrapper::IsInstanceOf(tjs_uint32 flag, const tjs_char* membername, tjs_uint32* hint, const tjs_char* classname, iTJSDispatch2* objthis) {

    // do noting
	return TJS_S_OK;
}

// -----------------------------------------------------------------

tTJSDispatch2Wrapper::tTJSDispatch2Wrapper(iTJSDispatch2* in_dispatch, iTJSDispatch2* in_dispatch_this) {

    dispatch = in_dispatch;
    dispatch_this = in_dispatch_this;

    if (dispatch) {
        dispatch->AddRef();
    }
    if (dispatch_this) {
        dispatch_this->AddRef();
    }
}

tTJSDispatch2Wrapper::~tTJSDispatch2Wrapper(void) {

    if (dispatch) {
        dispatch->Release();
    }
    if (dispatch_this) {
        dispatch_this->Release();
    }
}

iTJSDispatch2*
tTJSDispatch2Wrapper::GetNoAddRef(void) {

    return dispatch;
}

iTJSDispatch2*
tTJSDispatch2Wrapper::GetThisNoAddRef(void) {

    return dispatch_this;
}

// -----------------------------------------------------------------

tTJSWrapper::tTJSWrapper() {

    TVPLoadMessage();
    ScriptEngine = new tTJS();
}

tTJSWrapper::~tTJSWrapper() {

    ScriptEngine->Release();
}

tTJSDispatch2Wrapper*
tTJSWrapper::GetGlobal() {

    return new tTJSDispatch2Wrapper(ScriptEngine->GetGlobalNoAddRef(), ScriptEngine->GetGlobalNoAddRef());
}

void
tTJSWrapper::SetPPValue(const tjs_char* name, const tjs_int32 value) {

    ScriptEngine->SetPPValue(name, value);
}

tjs_int32
tTJSWrapper::GetPPValue(const tjs_char* name) {

    return ScriptEngine->GetPPValue(name);
}

void
tTJSWrapper::DoGarbageCollection(void) {

    ScriptEngine->DoGarbageCollection();
}

godot::Variant
tTJSWrapper::ExecScript(godot::Variant script, godot::Object context, godot::Variant name, godot::Variant lineofs) {

    tTJSVariant result;
    iTJSDispatch2* context_dispatch = NULL;

    ttstr tjs_script = Variant_GDtoTJS(script);
    ttstr tjs_name = Variant_GDtoTJS(name);
    tjs_int tjs_lineofs = Variant_GDtoTJS(lineofs);

    ScriptEngine->ExecScript(tjs_script,
                             &result,
                             context_dispatch,
                             tjs_name.GetLen() > 0 ? &tjs_name : NULL,
                             tjs_lineofs);

    return Variant_TJStoGD(result);
}

godot::Variant
tTJSWrapper::EvalExpression(godot::Variant script, godot::Object context, godot::Variant name, godot::Variant lineofs) {

    tTJSVariant result;
    iTJSDispatch2* context_dispatch = NULL;

    ttstr tjs_script = Variant_GDtoTJS(script);
    ttstr tjs_name = Variant_GDtoTJS(name);
    tjs_int tjs_lineofs = Variant_GDtoTJS(lineofs);

    ScriptEngine->EvalExpression(tjs_script,
                                 &result,
                                 context_dispatch,
                                 tjs_name.GetLen() > 0 ? &tjs_name : NULL,
                                 tjs_lineofs);

    return Variant_TJStoGD(result);
}

godot::Variant
tTJSWrapper::LoadByteCode(godot::Variant buff, godot::Object context, godot::Variant name) {

    tTJSVariant result;
    iTJSDispatch2* context_dispatch = NULL;

    ttstr tjs_buff = Variant_GDtoTJS(buff);
    ttstr tjs_name = Variant_GDtoTJS(name);

    ScriptEngine->LoadByteCode((const tjs_uint8 *)tjs_buff.c_str(),
                               tjs_buff.GetLen(),
                               &result, context_dispatch,
                               tjs_name.GetLen() > 0 ? tjs_name.c_str() : NULL);

    return Variant_TJStoGD(result);
}

godot::Variant
tTJSWrapper::CompileScript(godot::Variant script, godot::Variant isresultneeded, godot::Variant outputdebug, godot::Variant isexpression, godot::Variant name, godot::Variant lineofs) {
    
    ttstr tjs_script = Variant_GDtoTJS(script);
    ttstr tjs_name = Variant_GDtoTJS(name);
    tjs_int tjs_lineofs = Variant_GDtoTJS(lineofs);
    tTVPMemoryStream* outputstream = new tTVPMemoryStream;

    try {

        ScriptEngine->CompileScript(tjs_script.c_str(),
                                    outputstream,
                                    !!isresultneeded,
                                    !!outputdebug,
                                    !!isexpression,
                                    tjs_name.GetLen() > 0 ? tjs_name.c_str() : NULL,
                                    lineofs);
    }
    catch(...) {

        outputstream->Destruct();
        throw;
    }
    
    outputstream->Seek(0, TJS_BS_SEEK_SET);
    godot_pool_byte_array buffer;
    tjs_uint64 streamlen = outputstream->GetSize();
    outputstream->Read(&buffer, static_cast<tjs_uint>(streamlen));
    outputstream->Destruct();

    //return godot_pool_byte_array_write(&buffer);
    return 1;
}

}

// -----------------------------------------------------------------